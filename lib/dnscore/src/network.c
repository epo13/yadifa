/*------------------------------------------------------------------------------
 *
 * Copyright (c) 2011-2020, EURid vzw. All rights reserved.
 * The YADIFA TM software product is provided under the BSD 3-clause license:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *        * Redistributions of source code must retain the above copyright
 *          notice, this list of conditions and the following disclaimer.
 *        * Redistributions in binary form must reproduce the above copyright
 *          notice, this list of conditions and the following disclaimer in the
 *          documentation and/or other materials provided with the distribution.
 *        * Neither the name of EURid nor the names of its contributors may be
 *          used to endorse or promote products derived from this software
 *          without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *------------------------------------------------------------------------------
 *
 */

/** @defgroup
 *  @ingroup dnscore
 *  @brief
 *
 *
 *
 * @{
 *
 *----------------------------------------------------------------------------*/

#include "dnscore/dnscore-config.h"
#include "dnscore/network.h"

#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h>

bool
addr_info_is_any(struct addrinfo* addr)
{
    bool is_any;
    if(addr->ai_family == AF_INET6)
    {
        static const struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;
        const struct sockaddr_in6 *addr_v6 = (const struct sockaddr_in6*)addr->ai_addr->sa_data;
        is_any = memcmp(&addr_v6->sin6_addr, &in6addr_any, 16) == 0;
    }
    else if(addr->ai_family == AF_INET)
    {
        const struct sockaddr_in *addr_v4 = (const struct sockaddr_in*)addr->ai_addr->sa_data;
        is_any = (addr_v4->sin_addr.s_addr == INADDR_ANY);
    }
    else
    {
        // no supported, so no
        is_any = FALSE;
    }
    return is_any;
}

ya_result
network_interfaces_forall(network_interfaces_forall_callback cb, void *data)
{
#if 0 /* fix */
#elif 1
    ya_result ret = SUCCESS;
    struct ifaddrs *ia = NULL;
    if(getifaddrs(&ia) == 0)
    {
        socketaddress tmp;
        char tmp_name[128];
        for(struct ifaddrs *a = ia; a != NULL; a = a->ifa_next)
        {
            if((a->ifa_flags & IFF_UP) == 0)
            {
                // interface is down
                continue;
            }

            socketaddress* sa = (socketaddress*)a->ifa_addr;
            switch(sa->sa.sa_family)
            {
                case AF_INET:
                {
                    tmp = *sa;
                    strncpy(tmp_name, a->ifa_name, sizeof(tmp_name));
                    tmp_name[sizeof(tmp_name) - 1] = '\0';
                    //formatln("v4: %s: %{sockaddr}", tmp_name, &tmp);
                    ret = cb(tmp_name, &tmp, data);
                    break;
                }
                case AF_INET6:
                {
                    tmp = *sa;
                    strncpy(tmp_name, a->ifa_name, sizeof(tmp_name));
                    tmp_name[sizeof(tmp_name) - 1] = '\0';
                    //formatln("v6: %s: %{sockaddr}", tmp_name, &tmp);
                    ret = cb(tmp_name, &tmp, data);
                    break;
                }
                default:
                {
                    ret = SUCCESS; // ignore
                    break;
                }
            }

            if(FAIL(ret))
            {
                break;
            }
        }

        freeifaddrs(ia);
    }
    else
    {
        ret = ERRNO_ERROR;
    }

    return ret;
#endif

}

/** @} */
