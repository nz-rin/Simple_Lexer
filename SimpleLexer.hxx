#if not defined SIMPLE_LEXER_HXX
#define SIMPLE_LEXER_HXX

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>

#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <string_view>
#include <filesystem>

class SimpleLexer{
private:

	std::vector<std::string> _lex(std::string_view stream);

	inline std::vector<std::string> _open_and_lex_file(const char *file_path){

		int _file_fd = open(file_path, O_RDONLY);

		if(_file_fd == -1){
			std::cerr << "  [ERROR] Failed to open file: " << file_path << "\n  " << strerror(errno)<< std::endl;
			exit(EXIT_FAILURE);
		}

		struct stat _file_stat;
		if(fstat(_file_fd, &_file_stat) == -1){
			close(_file_fd);
			std::cerr << "  [ERROR] Failed to read info from file: " << file_path << "\n  " << strerror(errno)<< std::endl;
			exit(EXIT_FAILURE);
		}

		char *_buffer = (char *)mmap(NULL, static_cast<size_t>(_file_stat.st_size), PROT_READ, MAP_PRIVATE, _file_fd, 0);
		std::vector<std::string> _ret = _lex({_buffer, static_cast<size_t>(_file_stat.st_size)});

		//Clean up
		close(_file_fd);
		munmap(_buffer, static_cast<size_t>(_file_stat.st_size));
		return _ret;
	}

public:
	SimpleLexer() = default;
	SimpleLexer(const SimpleLexer &) = delete;
	SimpleLexer(const SimpleLexer &&) = delete;
	SimpleLexer & operator= (const SimpleLexer &) = delete;
	SimpleLexer & operator= (const SimpleLexer &&) = delete;

	inline std::vector<std::string> lex(const char *stream){ return _lex(stream);}
	inline std::vector<std::string> lex(std::string &stream){ return _lex(stream);}

	inline std::vector<std::string> lex(char *stream, size_t stream_size){ return _lex({stream, stream_size});}
	inline std::vector<std::string> lex(const char *stream, size_t stream_size){ return _lex({stream, stream_size});}


	inline std::vector<std::string> lex_file(const char *file_path){ return _open_and_lex_file(file_path);}
	inline std::vector<std::string> lex_file(std::string &file_path){ return _open_and_lex_file(file_path.c_str());}
	inline std::vector<std::string> lex_file(std::string file_path){ return _open_and_lex_file(file_path.c_str());}
	inline std::vector<std::string> lex_file(std::filesystem::path file_path){ return _open_and_lex_file(file_path.c_str());}
	inline std::vector<std::string> lex_file(std::filesystem::path &file_path){ return _open_and_lex_file(file_path.c_str());}

};

#if defined LEX_IMPL

#if not defined SINGLE_TOKENS
#define SINGLE_TOKENS "(){},.=+/*-"
#endif

#if not defined WHITESPACE_TOKENS
#define WHITESPACE_TOKENS "\n\t "
#endif

namespace _utils{

	inline bool _single_token(char c){
		const char * _single_tokens = SINGLE_TOKENS;
		while(*_single_tokens != '\0'){
			if (c == *_single_tokens++){
				return true;
			}
		}
		return false;
	}

	inline bool _whitespace_token(char c){
		const char * _whitespace = WHITESPACE_TOKENS;
		while(*_whitespace != '\0'){
			if (c == *_whitespace ++){
				return true;
			}
		}
		return false;
	}

	inline bool _numeric_token(char c){
		return (c >= '0' && c <= '9');
	}

	inline bool _alpha_token(char c){
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}

	inline bool _alpha_numeric_token(char c){
		return _alpha_token(c) || _numeric_token(c);
	}

};

std::vector<std::string> SimpleLexer::_lex(std::string_view stream){
	std::vector<std::string> _tokens = std::vector<std::string>();
	std::string _sb = ""; _sb.reserve(1024);
	size_t i = 0, _stream_size = stream.length();
	while(i < _stream_size){
		if( _utils::_whitespace_token(stream[i]) ){
			i++;
		}else if( _utils::_numeric_token(stream[i]) ){
			_sb.clear();
			while( _utils::_numeric_token(stream[i]) && i < _stream_size ){
				_sb += stream[i];
				i++;
			}
			_tokens.push_back(_sb);
		}else if ( _utils::_alpha_token(stream[i]) ){
			_sb.clear();
#if defined ALLOW_ALPHANUM
			while( _utils::_alpha_numeric_token(stream[i]) && i < _stream_size ){
#else
			while( _utils::_alpha_token(stream[i]) && i < _stream_size ){
#endif
				_sb += stream[i];
				i++;
			}
			_tokens.push_back(_sb);
		}else if( _utils::_single_token(stream[i]) ){
			_sb = stream[i];
			_tokens.push_back(_sb);
			i++;
		}else{
#if defined ABORT_ON_INVALID
			std::cerr << "  [ERROR] Invalid Token : \'" << stream[i] << "\' Index of : " << i << std::endl;
			exit(EXIT_FAILURE);
#else
			std::cerr << "  [WARNING] Unkown Token : \'" << stream[i] << "\' Index of : " << i << std::endl;
			i++;
#endif
		}
	}

	return _tokens;
}

#endif

#endif
