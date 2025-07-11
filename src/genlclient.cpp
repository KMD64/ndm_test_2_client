#include "genlclient.hpp"

#include <unistd.h>
#include <string.h>


#define MSG_BUFFER_SIZE 1024
namespace Mwl2
{

GenlClient::GenlClient ( uint32_t pid )
    :_sockFd ( -1 ),_srcAddr{AF_NETLINK,0,pid,0},_dstAddr{AF_NETLINK,0,0,0}
{
    
}

GenlClient::~GenlClient()
{
    if ( _sockFd>=0 )
        close ( _sockFd );
}

int GenlClient::exchange ( uint32_t destPid, const std::string &req, std::string &resp, int timeoutSec )
{
    long res = 0;
    if(_sockFd < 0 &&(res =_initConnection(destPid))<0)
    {
        return res;
    }
    printf("Sending %s\n",req.c_str());
    strcpy(reinterpret_cast<char*>(NLMSG_DATA(_phdr)),req.c_str());
    printf("Test\n");
    _iov.iov_len = _phdr->nlmsg_len = NLMSG_HDRLEN+req.size()+1;
    
    res = sendmsg(_sockFd,&_msgHdr,0);
    if(res < 0)
    {
        perror("sendmsg");
        return res;
    }
    
    resp.erase();
    //wait for the answer
    fd_set readSet;
    FD_SET(_sockFd,&readSet);
    
    timeval timeout{timeoutSec,0};
    res = select(_sockFd+1, &readSet, NULL,NULL,&timeout);
    if(!res)
    {
        return 1;
    }
    else if(res<0)
    {
        perror("select");
        return res;
    }
    
    _iov.iov_len = _phdr->nlmsg_len = NLMSG_SPACE(MSG_BUFFER_SIZE);
    res = recvmsg(_sockFd,&_msgHdr,0);
    if(res<0)
    {
        perror("recvmsg");
        return res;
    }
    resp.append(reinterpret_cast<char*>(NLMSG_DATA(_phdr)));
    
    return 0;
}

int GenlClient::_initConnection ( uint32_t destPid )
{
    _sockFd = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_GENERIC);
    if(_sockFd<0)
    {
        perror("socket");
        return _sockFd;
    }
    
    int res = bind(_sockFd,reinterpret_cast<sockaddr *>(&_srcAddr),sizeof(_srcAddr));
    if(res<0)
    {
        perror("bind");
        return res;
    }
    std::unique_ptr<char[]> buf(new char[NLMSG_SPACE(MSG_BUFFER_SIZE)]());
    memset(buf.get(),0,NLMSG_SPACE(MSG_BUFFER_SIZE));
    
    _buffer.swap(buf);
    _phdr = reinterpret_cast<nlmsghdr*>(_buffer.get());
    _phdr->nlmsg_len = NLMSG_SPACE(MSG_BUFFER_SIZE);
    _phdr->nlmsg_pid=100;
    _phdr->nlmsg_flags=0;
    
    _iov.iov_base = _phdr;
    _iov.iov_len = _phdr->nlmsg_len;
    
    _dstAddr.nl_pid = destPid;
    
    memset(&_msgHdr,0,sizeof(_msgHdr));
    _msgHdr.msg_name = &_dstAddr;
    _msgHdr.msg_namelen = sizeof(_dstAddr);
    _msgHdr.msg_iov = &_iov;
    _msgHdr.msg_iovlen = 1;
    
    return 0;
}
}
