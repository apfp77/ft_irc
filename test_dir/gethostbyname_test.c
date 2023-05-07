#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>


//   struct hostent {
// 	char	*h_name;	/* official name of host */
// 	char	**h_aliases;	/* alias list */
// 	int	h_addrtype;	/* host address type */
// 	int	h_length;	/* length of address */
// 	char	**h_addr_list;	/* list of addresses from name server */
// #if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
// #define	h_addr	h_addr_list[0]	/* address, for backward compatibility */
// #endif /* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */
// };

/*
  cc gethostbyname_test.c -o gethostbyname

  ./gethostbyname naver.com
  ./gethostbyname 8.8.8.8
*/

int main(int argc, char **argv)
{
    struct hostent     *myent;
    struct in_addr  myen;
    long int *add;

    myent = gethostbyname(argv[1]);
    if (myent == NULL)
    {
      perror("ERROR : ");
      exit(0);
    }
    printf("h_name: %s\n", myent->h_name);
    /*
      error, 뭔지 모르겠음
      int i = 0;
      while (myent->h_aliases != NULL)
      {
        printf("%s\n", myent->h_aliases[i]);
        i++;
      }
    */
    printf("h_addrtype: %d\n", myent->h_addrtype);
    printf("h_length: %d\n", myent->h_length);

    while(*myent->h_addr_list != NULL)
    {
        add = (long int *)*myent->h_addr_list;
        myen.s_addr = *add;
        printf("%s\n", inet_ntoa(myen));
        myent->h_addr_list++;
    }
}