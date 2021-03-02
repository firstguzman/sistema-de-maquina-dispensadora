#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

typedef struct date Date;
typedef struct hour Hour;

//STRUCTS UTILIZADOS DENTRO DEL SISTEMA
struct hour{
    int hour;
    int minute;
    int second;
};
struct date{
    int day;
    int month;
    int year;
    Hour hour;
};
struct reporte{
  char fecha[100];
  char producto[18];
};

//VARIABLES GLOBALES DEL SISTEMA
int cartera[6] = {10,10,10,5,5,3}; //vector con el dinero de la maquina
int cantidad[4] = {5,5,5,5}; //vector con el precio de los productos en la maquina
struct reporte reportes[10];

void tiempo_espera() {
  int segundos;
  segundos = 0;
  printf("\nSu producto saldr%c en:",160);

  while (segundos < 7){
    Sleep(1000);
    segundos++;
    printf("\n%02d", segundos);
  }

  printf("\n\n\t\t%cCOMPRA PROCESADA%c",173,33);
}

float valor_producto(int aux) {
  if (aux == 1) return 1.15;
  else if (aux == 2) return 1.25;
  else if (aux == 3) return 1.35;
  else if (aux == 4) return 0.95;
}

int validar_disponibilidad(int cantidad[4], int aux) {
  for (int i = 0; i < 4; i++) {
    if (i == aux-1) {
      if (cantidad[i]>0) {
        return 1;
      } else {
        return 0;
      }
    }
  }
}

int validar_dinero_ingresado(float dinero, int op) {
  //Validar el dinero ingresado con el costo del producto. Si no es suficiente, retorna 0
  if(((op == 1) && dinero < valor_producto(op)) || ((op == 2) && (dinero < valor_producto(op))) || ((op == 3) && (dinero < valor_producto(op)))
   || ((op == 4) && (dinero < valor_producto(op)))) {
    return 0;
  }
  else {
    return 1;
  }
}

float suma_dinero(int monedas[6]) {
  float suma = 0;
  suma = suma + (monedas[0] * 0.01);
  suma = suma + (monedas[1] * 0.05);
  suma = suma + (monedas[2] * 0.10);
  suma = suma + (monedas[3] * 0.25);
  suma = suma + (monedas[4] * 0.50);
  suma = suma + (monedas[5] * 1);
  return suma;
}

void disminuir_producto(int cantidad[4], int op) {
  if(op == 1) {
    cantidad[0]--;
  }
  else if(op == 2) {
    cantidad[1]--;
  }
  else if(op == 3) {
    cantidad[2]--;
  }
  else if(op == 4) {
    cantidad[3]--;
  }
}

void descontar_monedas(float cambio, int cartera[6]) {
  while (cambio) {
    if((cambio >= 1) && (cartera[5])){
      cambio = cambio - 1;
      cartera[5] = cartera[5] - 1;
    }
    else if((cambio >= 0.5) && (cartera[4])){
      cambio = cambio - 0.5;
      cartera[4] = cartera[4] - 0.5;
    }
    else if((cambio >= 0.25) && (cartera[3])){
      cambio = cambio - 0.25;
      cartera[3] = cartera[3] - 0.25;
    }
    else if((cambio >= 0.10) && (cartera[2])){
      cambio = cambio - 0.10;
      cartera[2] = cartera[2] - 0.10;
    }
    else if((cambio >= 0.05) && (cartera[1])){
      cambio = cambio - 0.05;
      cartera[1] = cartera[1] - 0.05;
    }
    else if((cambio >= 0.01) && (cartera[0])){
      cambio = cambio - 0.01;
      cartera[0] = cartera[0] - 0.01;
    }
    else {
      cambio = 0;
    }
  }
}

void mostrar_producto(int op){
  if(op == 1){
    printf( "\nProducto comprado: (1) USM Coke\n ");
  }
  else if(op == 2) {
    printf( "\nProducto comprado: (2) USM Casera Limon\n ");
  }
  else if(op == 3) {
    printf( "\nProducto comprado: (3) USMRADE\n ");
  }
  else if(op == 4) {
    printf( "\nProducto comprado: (4) USM Water\n ");
  }
}

void registrar_compra(int op, struct reporte reportes[10]){
  Date date;

  char buff[100];
  time_t now = time(0);
  struct tm now_t = *localtime(&now);
  strftime (buff, 100, "%d-%m-%Y %H:%M:%S", &now_t);

  date.year = now_t.tm_year + 1900;
  date.month = now_t.tm_mon + 1;
  date.day = now_t.tm_mday;

  date.hour.hour = now_t.tm_hour;
  date.hour.minute = now_t.tm_min;
  date.hour.second = now_t.tm_sec;

  char aux_producto[18];
  if(op == 1){
    strcpy(aux_producto,"USM Coke");
  }
  else if(op == 2) {
    strcpy(aux_producto,"USM Casera Limon");
  }
  else if(op == 3) {
    strcpy(aux_producto,"USMRADE");
  }
  else if(op == 4) {
    strcpy(aux_producto,"USM Water");
  }

  for (int i = 0; i < 10; i++) {
    if(reportes[i].fecha[0] == '\0') {
      strcpy(reportes[i].fecha, buff);
      strcpy(reportes[i].producto, aux_producto);
      break;
    }
  }
}

void comprar(int cantidad[4], int cartera[6], int aux, struct reporte reportes[10]) {
    //Validar si hay disponibilidad del producto
    if (validar_disponibilidad(cantidad, aux)) {
      //Leemos el archivo .txt para saber cuántas y cuáles monedas se ingresan
      FILE *archivo;
      char caracteres[20];

      //Metemos las monedas ingresadas en un array
      int monedas[6];
      int pos_moneda = 0;

	    archivo = fopen("monedas.txt","r");

      if (archivo == NULL) {
        printf("\nError en la apertura del archivo.\n\n");
      }
      else {

        while(feof(archivo) == 0) {
          fgets(caracteres,20,archivo);
          monedas[pos_moneda] = atoi(caracteres);
          pos_moneda++;
        }
      }
	    fclose(archivo);

      float dinero_total = suma_dinero(monedas);

      //Dinero total insertado
      if (dinero_total >= 1) {
        printf("\nLa cantidad de dinero ingresado es: %5.2f dollar", dinero_total);
      }else{
        printf("\nLa cantidad de dinero ingresado es: %5.2f cent", dinero_total);
      }

      //Si el dinero ingresado es suficiente, entonces se suman las monedas ingresadas y se reduce la cantidad del producto
      if(validar_dinero_ingresado(dinero_total, aux)) {
        //Sumar monedas ingresadas
        for (int i = 0; i < 6; i++) {
          cartera[i] = cartera[i] + monedas[i];
        }

        //Verifica de cuánto es el cambio que tiene que devolver
        float cambio = dinero_total - valor_producto(aux);
        float devolver_cambio = cambio;

        int monedas_cambio[6] = {0,0,0,0,0,0};
        int aux_cartera[6];
        for (int i = 0; i < 6; i++) {
          aux_cartera[i] = cartera[i];
        }

        bool cambio_suficiente = true;

        //Manejar la lógica para llenar el array de las monedas con el cambio
        //Verificamos si hay suficientes monedas para proceder a dar el cambio, si no las hay, entonces se le notifica al usuario si desea las monedas existentes, sino las quiere se le devuelve el dinero
        while (cambio){
          //1 dólar
          if(cambio < 1) {
            //50 centavos
            if(cambio < 0.5) {
              //25 centavos
              if(cambio < 0.25) {
                //10 centavos
                if(cambio < 0.10) {
                  //5 centavos
                  if(cambio < 0.05) {
                    //1 centavo
                    if(cambio < 0.01) {
                      cambio = 0;
                    }
                    else if(aux_cartera[0]) {
                      aux_cartera[0]--;
                      cambio = cambio - 0.01;
                      monedas_cambio[0]++;
                    }
                    else {
                      cambio_suficiente = false;
                      break;
                    }
                  }
                  else if(aux_cartera[1]) {
                    aux_cartera[1]--;
                    cambio = cambio - 0.05;
                    monedas_cambio[1]++;
                  }
                  else {
                    if(aux_cartera[0]) {
                      aux_cartera[0]--;
                      cambio = cambio - 0.01;
                      monedas_cambio[0]++;
                    }
                    else {
                      cambio_suficiente = false;
                      break;
                    }
                  }
                }
                else if(aux_cartera[2]) {
                  aux_cartera[2]--;
                  cambio = cambio - 0.10;
                  monedas_cambio[2]++;
                }
                else {
                  if(aux_cartera[1]) {
                    aux_cartera[1]--;
                    cambio = cambio - 0.05;
                    monedas_cambio[1]++;
                  }
                  else if(aux_cartera[0]) {
                    aux_cartera[0]--;
                    cambio = cambio - 0.01;
                    monedas_cambio[0]++;
                  }
                  else {
                    cambio_suficiente = false;
                    break;
                  }
                }
              }
              else if(aux_cartera[3]) {
                aux_cartera[3]--;
                cambio = cambio - 0.25;
                monedas_cambio[3]++;
              }
              else {
                if(aux_cartera[2]) {
                  aux_cartera[2]--;
                  cambio = cambio - 0.10;
                  monedas_cambio[2]++;
                }
                else if(aux_cartera[1]) {
                  aux_cartera[1]--;
                  cambio = cambio - 0.05;
                  monedas_cambio[1]++;
                }
                else if(aux_cartera[0]) {
                  aux_cartera[0]--;
                  cambio = cambio - 0.01;
                  monedas_cambio[0]++;
                }
                else {
                  cambio_suficiente = false;
                  break;
                }
              }
            }
            else if(aux_cartera[4]) {
              aux_cartera[4]--;
              cambio = cambio - 0.5;
              monedas_cambio[4]++;
            }
            else {
              if(aux_cartera[3]) {
                aux_cartera[3]--;
                cambio = cambio - 0.25;
                monedas_cambio[3]++;
              }
              else if(aux_cartera[2]) {
                aux_cartera[2]--;
                cambio = cambio - 0.10;
                monedas_cambio[2]++;
              }
              else if(aux_cartera[1]) {
                aux_cartera[1]--;
                cambio = cambio - 0.05;
                monedas_cambio[1]++;
              }
              else if(aux_cartera[0]) {
                aux_cartera[0]--;
                cambio = cambio - 0.01;
                monedas_cambio[0]++;
              }
              else {
                cambio_suficiente = false;
                break;
              }
            }
          }
          else if(aux_cartera[5]) {
            aux_cartera[5]--;
            cambio = cambio - 1;
            monedas_cambio[5]++;
          }
          //Si no tengo monedas de 1, busco entre las demás
          else {
            if(aux_cartera[4]) {
              aux_cartera[4]--;
              cambio = cambio - 0.5;
              monedas_cambio[4]++;
            }
            else if(aux_cartera[3]) {
              aux_cartera[3]--;
              cambio = cambio - 0.25;
              monedas_cambio[3]++;
            }
            else if(aux_cartera[2]) {
              aux_cartera[2]--;
              cambio = cambio - 0.10;
              monedas_cambio[2]++;
            }
            else if(aux_cartera[1]) {
              aux_cartera[1]--;
              cambio = cambio - 0.05;
              monedas_cambio[1]++;
            }
            else if(aux_cartera[0]) {
              aux_cartera[0]--;
              cambio = cambio - 0.01;
              monedas_cambio[0]++;
            }
            else {
              cambio_suficiente = false;
              break;
            }
          }
        }

        int continuar;
        if(!cambio_suficiente) {
          printf("\nNo hay suficientes monedas para el cambio\n");
          printf("\nEl m%dximo cambio que se le puede dar es de:\n",160);

          printf("\n\n%cDesea continuar con la operaci%cn%c\n",168,162,63);
          printf("\n\nPresione 0 para abortar, 1 para continuar\n");
          scanf("%i", &continuar);

          if(continuar){
            descontar_monedas(devolver_cambio, cartera);
            //Disminuye producto
            disminuir_producto(cantidad, aux);

            //Registrar compra para el reporte
            registrar_compra(aux, reportes);

            //Tiempo de espera de 7 segundos
            tiempo_espera();

            //Mostrar producto entregado
            mostrar_producto(aux);

            //Indicar dinero retornado en cada monera
            if (devolver_cambio > round(0)) {
              printf("\n\t\tCAMBIO OTORGADO:");
              printf("\nCentavos de 0,01: %i", monedas_cambio[0]);
              printf("\nCentavos de 0,05: %i", monedas_cambio[1]);
              printf("\nCentavos de 0,10: %i", monedas_cambio[2]);
              printf("\nCentavos de 0,25: %i", monedas_cambio[3]);
              printf("\nCentavos de 0,50: %i", monedas_cambio[4]);
              printf("\nDolar de 1: %i\n", monedas_cambio[5]);

              printf("\nCambio total: %5.2f\n", devolver_cambio);
            }

            printf( "\n\t\t%cGRACIAS POR SU COMPRA%c\n\n",173,33);

            system("pause");
            mainCompra(cantidad, cartera, reportes);
          }
        }
        //Sí hay cambio suficiente, se procede a otorgar el producto junto al cambio
        else {
          descontar_monedas(devolver_cambio, cartera);
          //Disminuye producto
          disminuir_producto(cantidad, aux);

          //Registrar compra para el reporte
          registrar_compra(aux, reportes);

          //Tiempo de espera de 7 segundos
          tiempo_espera();

          //Mostrar producto entregado
          mostrar_producto(aux);

          //Indicar dinero retornado en cada monera
          if (devolver_cambio > round(0)) {
            printf("\n\t\tCAMBIO OTORGADO:");
            printf("\nCentavos de 0,01: %i", monedas_cambio[0]);
            printf("\nCentavos de 0,05: %i", monedas_cambio[1]);
            printf("\nCentavos de 0,10: %i", monedas_cambio[2]);
            printf("\nCentavos de 0,25: %i", monedas_cambio[3]);
            printf("\nCentavos de 0,50: %i", monedas_cambio[4]);
            printf("\nDolar de 1: %i\n", monedas_cambio[5]);

            printf("\nCambio total: %5.2f\n", devolver_cambio);
          }

            printf( "\n\t\t%cGRACIAS POR SU COMPRA%c\n\n",173,33);

          system("pause");
          mainCompra(cantidad, cartera, reportes);
        }
      }
      else {
        printf("\n\n\t\t%cCOMPRA NO PROCESADA%c",173,33);
        printf("\nEl dinero ingresado es insuficiente\n\n");

        system("pause");
        mainCompra(cantidad, cartera, reportes);
      }
    }
    else{
      printf( "Producto no disponible\n ");
      system("pause");
      mainCompra(cantidad, cartera, reportes);
    }
}

void mostrar_cartera (int cartera[6]){
    printf( "\n\n\t\tMONEDAS DISPONIBLES EN LA M%cQUINA\n",181);
    printf("Disponible %d monedas de 1 cent\n", cartera[0]);
    printf("Disponible %d monedas de 5 cent\n", cartera[1]);
    printf("Disponible %d monedas de 10 cent\n", cartera[2]);
    printf("Disponible %d monedas de 25 cent\n", cartera[3]);
    printf("Disponible %d monedas de 50 cent\n", cartera[4]);
    printf("Disponible %d monedas de 1 dollar\n", cartera[5]);
}

void mainCompra(int cantidad[4], int cartera[6], struct reporte reportes[10]) {
    system("cls");
    int op = -1, x = 0;

    mostrar_cartera(cartera);

    do {
      printf( "\n\n\t\tPRODUCTOS DISPONIBLES \n ");
      printf( "1.\t%d USM Coke --> 1.15$\n ", cantidad[0]);
      printf( "2.\t%d USM Casera Limon --> 1.25$\n ", cantidad[1]);
      printf( "3.\t%d USMRADE --> 1.35$\n ", cantidad[2]);
      printf( "4.\t%d USM Water --> 0.95$\n ", cantidad[3]);
      printf( "5.\tVolver\n ");

      scanf("%i", &op);

      switch (op) {
        case 1: comprar(cantidad, cartera, op, reportes);
        break;
        case 2: comprar(cantidad, cartera, op, reportes);
        break;
        case 3: comprar(cantidad, cartera, op, reportes);
        break;
        case 4: comprar(cantidad, cartera, op, reportes);
        break;
        case 5: main();
        return;
      }
    } while( op < 1 || op > 4 );
}

void mainReporte(struct reporte reportes[10]) {
  char password[4];

    printf( "\n\nIntroduzca la clave de acceso\n ");
    scanf("%s", &password);

    if (strcmp(password, "USM") == 0) {
      printf("\n\n\t\t%cLTIMAS 10 TRANSACCIONES\n",233);
      for (int i = 0; i < 10; i++) {
        if(reportes[i].fecha[0]){
          printf("\nProducto comprado: %s. Fecha de compra: %s.", reportes[i].producto, reportes[i].fecha);
        }
      }
       printf("\n");
    }
    else{
        printf("\n\n\t\tClave invalida\n");
        main();
    }
}


int main() {

    int op = -1;
    while (op) {
      system("cls");
      printf( "\n\n\t\tBIENVENIDOS A USM DRY \n ");
  		printf( "1.\tComprar\n ");
  		printf( "2.\tReportes\n ");
  		printf( "0.\tSALIR del sistema\n\n ");
  		scanf("%i", &op);

      switch (op){
  			case 1: mainCompra(cantidad, cartera, reportes);
  					break;
  			case 2: mainReporte(reportes);
  					break;
  		}
  		system("pause");
    }
  return 0;
};
