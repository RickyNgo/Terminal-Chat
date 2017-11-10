//
// chat_message.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

class chat_message
{
public:
  enum { header_length = 6 };
  enum { max_body_length = 255 };

  chat_message()
    : body_length_(0), id_(0)
  {
  }

  const char* data() const
  {
    return data_;
  }

  char* data()
  {
    return data_;
  }

  std::size_t length() const
  {
    return header_length + body_length_;
  }

  const char* body() const
  {
    return data_ + header_length;
  }

  char* body()
  {
    return data_ + header_length;
  }

  std::size_t body_length() const
  {
    return body_length_;
  }

  void body_length(std::size_t new_length)
  {
    body_length_ = new_length;
    if (body_length_ > max_body_length)
      body_length_ = max_body_length;
  }

  bool decode_header()
  {
    char header[header_length + 1] = "";
    std::strncat(header, data_, header_length );
    uint8_t cmd;
    body_length_ = cmd = 0;
    body_length_ |= header[ 0 ];
    cmd          |= header[ 1 ];
    id_           = std::atoi( header + 2 );
    printf("%d\n", body_length_ );
    printf("%d\n", cmd );
    if (body_length_ > max_body_length)
    {
      body_length_ = 0;
      return false;
    }
    return true;
  }

  void encode_header()
  {
    char header[header_length + 1] = "";
    header[ 0 ] |= static_cast<uint8_t>( body_length());
    header[ 1 ] |= static_cast<uint8_t>( 1 );
    sprintf( header + 2, "%d", id_ );
    std::memcpy(data_, header, header_length );
  }

private:
  char              data_[header_length + max_body_length];
  uint8_t           body_length_;
  int               id_;
};

#endif // CHAT_MESSAGE_HPPc