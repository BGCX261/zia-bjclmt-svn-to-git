

#ifndef THREAD_EXCEPTION_H_
# define THREAD_EXCEPTION_H_

#include <iostream>
#include <exception>

class		threadException : public std::exception
{
 public:
  threadException(const char *msg) throw()
    {
      this->_Message = msg;
    }
  ~threadException() throw()
    {
    }
  const char	*what(void) const throw()
  {
    return (this->_Message.c_str());
  }
  
 private:
  std::string	_Message;
};

#endif //THREAD_EXCEPTION_H_
