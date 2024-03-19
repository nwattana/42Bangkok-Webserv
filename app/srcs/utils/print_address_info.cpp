#include <arpa/inet.h>
#include "Webserv.hpp"

std::string sockettypestr(int type)
{
	if (type == SOCK_STREAM)
		return "SOCK_STREAM";
	else if (type == SOCK_DGRAM)
		return "SOCK_DGRAM";
	else if (type == SOCK_RAW)
		return "SOCK_RAW";
	else if (type == SOCK_RDM)
		return "SOCK_RDM";
	else if (type == SOCK_SEQPACKET)
		return "SOCK_SEQPACKET";
	else
		return "UNKNOWN";
}

std::string protocolstr(int protocol)
{
	switch (protocol) {
		case IPPROTO_IP: return "IPPROTO_IP";
		case IPPROTO_ICMP: return "IPPROTO_ICMP";
		case IPPROTO_IGMP: return "IPPROTO_IGMP";
		case IPPROTO_IPIP: return "IPPROTO_IPIP";
		case IPPROTO_TCP: return "IPPROTO_TCP";
		case IPPROTO_EGP: return "IPPROTO_EGP";
		case IPPROTO_PUP: return "IPPROTO_PUP";
		case IPPROTO_UDP: return "IPPROTO_UDP";
		case IPPROTO_IDP: return "IPPROTO_IDP";
		case IPPROTO_TP: return "IPPROTO_TP";
		case IPPROTO_DCCP: return "IPPROTO_DCCP";
		case IPPROTO_IPV6: return "IPPROTO_IPV6";
		case IPPROTO_ROUTING: return "IPPROTO_ROUTING";
		case IPPROTO_FRAGMENT: return "IPPROTO_FRAGMENT";
		case IPPROTO_RSVP: return "IPPROTO_RSVP";
		case IPPROTO_GRE: return "IPPROTO_GRE";
		case IPPROTO_ESP: return "IPPROTO_ESP";
		case IPPROTO_AH: return "IPPROTO_AH";
		case IPPROTO_ICMPV6: return "IPPROTO_ICMPV6";
		case IPPROTO_NONE: return "IPPROTO_NONE";
		case IPPROTO_DSTOPTS: return "IPPROTO_DSTOPTS";
		case IPPROTO_MTP: return "IPPROTO_MTP";
		case IPPROTO_ENCAP: return "IPPROTO_ENCAP";
		case IPPROTO_PIM: return "IPPROTO_PIM";
		case IPPROTO_COMP: return "IPPROTO_COMP";
		case IPPROTO_SCTP: return "IPPROTO_SCTP";
		case IPPROTO_UDPLITE: return "IPPROTO_UDPLITE";
		case IPPROTO_RAW: return "IPPROTO_RAW";
		default: return "UNKNOWN";
	}
}

std::string familytostr(int ai_family)
{
	switch (ai_family) {
		case AF_UNSPEC: return "AF_UNSPEC";
		case AF_INET: return "AF_INET";
		case AF_INET6: return "AF_INET6";
		case AF_UNIX: return "AF_UNIX or AF_LOCAL";
		case AF_ROUTE: return "AF_ROUTE or AF_NETLINK";
		case AF_KEY: return "AF_KEY";
		case AF_X25: return "AF_X25";
		case AF_AX25: return "AF_AX25";
		case AF_ATMPVC: return "AF_ATMPVC";
		case AF_APPLETALK: return "AF_APPLETALK";
		case AF_PACKET: return "AF_PACKET";
		case AF_ALG: return "AF_ALG";
		case AF_BLUETOOTH: return "AF_BLUETOOTH";
		case AF_BRIDGE: return "AF_BRIDGE";
		case AF_ECONET: return "AF_ECONET";
		case AF_IEEE802154: return "AF_IEEE802154";
		case AF_IRDA: return "AF_IRDA";
		case AF_LLC: return "AF_LLC";
		case AF_PPPOX: return "AF_PPPOX";
		case AF_RDS: return "AF_RDS";
		case AF_ROSE: return "AF_ROSE";
		case AF_SECURITY: return "AF_SECURITY";
		case AF_TIPC: return "AF_TIPC";
		case AF_VSOCK: return "AF_VSOCK";
		case AF_WANPIPE: return "AF_WANPIPE";
		default: return "UNKNOWN";
	}

}

void printAddressInfo(struct addrinfo *p)
{
	char ipstr[INET6_ADDRSTRLEN];
	void *addr;
	std::string ipver;
	struct addrinfo *serverinfo = p; // will point to the results

	while (serverinfo != NULL)
	{

		// get the pointer to the address itself,
		// different fields in IPv4 and IPv6:
		if (serverinfo->ai_family == AF_INET) { // IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)serverinfo->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver="IPv4";
		}
		else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)serverinfo->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver="IPv6";
		}

		// convert the IP to a string and print it:
		inet_ntop(serverinfo->ai_family, addr, ipstr, sizeof(ipstr));
		std::cout << "  " << ipver << ": " << ipstr << std::endl;

		// show content in struct
		std::cout << "  ai_family: " << familytostr(serverinfo->ai_family) << std::endl;
		std::cout << "  ai_socktype: " << sockettypestr(serverinfo->ai_socktype) << std::endl;
		std::cout << "  ai_protocol: " << protocolstr(serverinfo->ai_protocol) << std::endl;
		std::cout << "  ai_addrlen: " << serverinfo->ai_addrlen << std::endl;
		if (serverinfo->ai_canonname)
			std::cout << "  ai_canonname: " << serverinfo->ai_canonname << std::endl;
		else
			std::cout << "  ai_canonname: NULL" << std::endl;
		std::cout << "  ai_addr: " << serverinfo->ai_addr << std::endl;
		std::cout << "====================\n";
		// just linked list
		// std::cout << "  ai_next: " << serverinfo->ai_next << std::endl;
		serverinfo = serverinfo->ai_next;
	}
}