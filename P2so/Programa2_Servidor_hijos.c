#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  int udp_socket, lbind, tam, lrecv;
  int n = 1;
  struct sockaddr_in SERVIDOR, CLIENTE;
  unsigned char msj[512];
  unsigned char paqRec[512];
  udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

  if (udp_socket == -1)
  {
    perror("\nError al abrir el socket");
    exit(0);
  }
  else
  {
    perror("\nExito al abrir el socket");
    SERVIDOR.sin_family = AF_INET;
    SERVIDOR.sin_port = htons(8080);       /*Puerto elegible*/
    SERVIDOR.sin_addr.s_addr = INADDR_ANY; /*Para que agarre la dirección ip automáticamente*/
    ;
    lbind = bind(udp_socket, (struct sockaddr *)&SERVIDOR, sizeof(SERVIDOR));

    if (lbind == -1)
    {
      perror("\nError en bind");
      exit(0);
    }
    else
    {
      perror("\nExito en bind");
      lrecv = sizeof(CLIENTE);
      //inicio de bucle
      while (n != 0)
      {
        tam = recvfrom(udp_socket, paqRec, 512, 0, (struct sockaddr *)&CLIENTE, &lrecv);
        if (tam == -1)
        {
          perror("\nError al recibir");
          exit(0);
        }
        else
        {
          printf("\nCliente: %s", paqRec);
          //Pido al usuario el mensaje
          printf("\nServidor:");
          fgets(msj, 512, stdin);
          //Envio el mensaje
          tam = sendto(udp_socket, msj, strlen(msj) + 1, 0, (struct sockaddr *)&CLIENTE, sizeof(CLIENTE));

          if (tam == -1)
          {
            perror("\nError al enviar");
            exit(0);
          }
          else
          {
          }
        }
      }
    }
  }

  close(udp_socket); //cierre de socket
  return 0;
}