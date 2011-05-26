/*  vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
 * 
 *  Gearmand client and server library.
 *
 *  Copyright (C) 2011 Data Differential, http://datadifferential.com/
 *  Copyright (C) 2008 Brian Aker, Eric Day
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *      * Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *
 *      * Redistributions in binary form must reproduce the above
 *  copyright notice, this list of conditions and the following disclaimer
 *  in the documentation and/or other materials provided with the
 *  distribution.
 *
 *      * The names of its contributors may not be used to endorse or
 *  promote products derived from this software without specific prior
 *  written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#pragma once

#include <libgearman/connection.h>

struct gearman_connection_st
{
  struct {
    bool ready;
    bool packet_in_use;
    bool ignore_lost_connection;
    bool close_after_flush;
  } options;
  enum gearman_con_universal_t state;
  enum gearman_con_send_t send_state;
  enum gearman_con_recv_t recv_state;
  in_port_t port;
  short events;
  short revents;
  int fd;
  uint32_t created_id;
  uint32_t created_id_next;
  size_t send_buffer_size;
  size_t send_data_size;
  size_t send_data_offset;
  size_t recv_buffer_size;
  size_t recv_data_size;
  size_t recv_data_offset;
  gearman_universal_st &universal;
  gearman_connection_st *next;
  gearman_connection_st *prev;
  void *context;
  struct addrinfo *addrinfo;
  struct addrinfo *addrinfo_next;
  char *send_buffer_ptr;
  gearman_packet_st *recv_packet;
  char *recv_buffer_ptr;
  gearman_packet_st _packet;
  char host[NI_MAXHOST];
  char send_buffer[GEARMAN_SEND_BUFFER_SIZE];
  char recv_buffer[GEARMAN_RECV_BUFFER_SIZE];

  gearman_connection_st(gearman_universal_st &universal_arg,
                        gearman_connection_options_t *options);

  ~gearman_connection_st();

  void set_host( const char *host, const in_port_t port);

  gearman_return_t send(const gearman_packet_st&, const bool flush_buffer);
  size_t send(const void *data, size_t data_size, gearman_return_t *ret_ptr);

  gearman_return_t flush();
  void close();

  // Receive packet from a connection.
  gearman_packet_st *receiving(gearman_packet_st&,
                               gearman_return_t& , const bool recv_data);

  // Receive packet data from a connection.
  size_t receiving(void *data, size_t data_size, gearman_return_t&);

  // Set events to be watched for a connection.
  void set_events(short events);

 // Set events that are ready for a connection. This is used with the
 // external event callbacks.
  void set_revents(short revents);

  void reset_addrinfo();

private:
  size_t recv(void *data, size_t data_size, gearman_return_t&);
};

/**
 * Initialize a connection structure. Always check the return value even if
 * passing in a pre-allocated structure. Some other initialization may have
 * failed.
 */

GEARMAN_LOCAL
gearman_connection_st *gearman_connection_create(gearman_universal_st &universal,
                                                 gearman_connection_options_t *options);

GEARMAN_LOCAL
gearman_connection_st *gearman_connection_copy(gearman_universal_st& universal,
                                               const gearman_connection_st& from);

/**
 * Create a connection structure with the given host and port.
 *
 * @param[in] gearman Structure previously initialized with gearman_create() or
 *  gearman_clone().
 * @param[in] connection Caller allocated structure, or NULL to allocate one.
 * @param[in] host Host or IP address to connect to.
 * @param[in] port Port to connect to.
 * @return On success, a pointer to the (possibly allocated) structure. On
 *  failure this will be NULL.
 */
GEARMAN_LOCAL
gearman_connection_st *gearman_connection_create_args(gearman_universal_st &universal,
                                                      const char *host, in_port_t port);
