#ifndef MWL2_GENLCLIENT_H
#define MWL2_GENLCLIENT_H

#include <sys/socket.h>
#include <linux/netlink.h>
#include <string>
#include <memory>

namespace Mwl2 
{


class GenlClient
{
    
private:
    int _sockFd;
    std::unique_ptr<char[]> _buffer;
    nlmsghdr *_phdr;
    iovec _iov;
    msghdr _msgHdr;
    sockaddr_nl _srcAddr;
    sockaddr_nl _dstAddr;
    
    int _initConnection(uint32_t destPid);
public:

    GenlClient(uint32_t pid=0);
    ~GenlClient();
    
    int exchange(uint32_t destPid, const std::string &req,std::string &resp, int timeoutSec);
};

}

#endif // MWL2_GENLCLIENT_H
