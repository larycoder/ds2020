struct dest_host{
  string host<>;
  int port;
};

typedef char content<100>;

struct p_message{
  content ct;
  int length;
  int fd;
};

program rpc_proxy_program{
  version rpc_proxy_ver{
    int connect_proxy(dest_host) = 1;
    int send_proxy(p_message) = 2;
    p_message recv_proxy(int) = 3;
    int close_proxy(int) = 4;
  } = 1;
} = 0x40102103;