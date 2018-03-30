
 
#include <SPI.h>  //Importamos librería comunicación SPI
#include <Ethernet.h>  //Importamos librería Ethernet
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };//Ponemos la dirección MAC de la Ethernet Shield
IPAddress ip(192,168,0,50); //Asingamos la IP al Arduino
IPAddress gateway(192,168,0, 1); // Puerta de enlace o Router
IPAddress subnet(255, 255, 255, 0); //Mascara de Sub Red
EthernetServer server(80); //Creamos un servidor Web con el puerto 80 que es el puerto HTTP por defecto
 
int lampara=2; //Pin de Arduino asignado a la lampara
int lampara2=3; //Pin de Arduino asignado al ventilador
String estado1="OFF"; //Estado de la Lampara inicialmente en "OFF"
String estado2="OFF"; //Estado del ventilador inicialmente en "OFF"
 
void setup()
{
  Serial.begin(9600);
 
  // Inicializamos la comunicación Ethernet y el servidor
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Server Ok ");
  Serial.println(Ethernet.localIP());
 
  pinMode(lampara,OUTPUT);
  pinMode(lampara2,OUTPUT);
}
 
void loop()
{
  EthernetClient client = server.available(); //Creamos un cliente Web
  //Cuando detecte un cliente a través de una petición HTTP
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true; //Una petición HTTP acaba con una línea en blanco
    String cadena=""; //Creamos una cadena de caracteres vacía
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();//Leemos la petición HTTP carácter por carácter
        
        Serial.write(c);//Visualizamos la petición HTTP por el Monitor Serial
        cadena.concat(c);//Unimos el String 'cadena' con la petición HTTP (c). De esta manera convertimos la petición HTTP a un String
 
         //Ya que hemos convertido la petición HTTP a una cadena de caracteres, ahora podremos buscar partes del texto.
         int posicion1=cadena.indexOf("LAMPARA="); //Guardamos la posición de la instancia "LAMPARA=" a la variable 'posicion1'
 
          if(cadena.substring(posicion1)=="LAMPARA=ON")//Si a la posición 'posicion1' hay "LAMPARA=ON"
          {
            digitalWrite(lampara,HIGH);
            estado1="ON";
          }
          if(cadena.substring(posicion1)=="LAMPARA=OFF")//Si a la posición 'posicion1' hay "LAMPARA=OFF"
          {
            digitalWrite(lampara,LOW);
            estado1="OFF";
          }
 
         int posicion2=cadena.indexOf("LAMPARA2="); //Guardamos la posición de la instancia "VENTILADOR=" a la variable 'posicion2'
 
          if(cadena.substring(posicion2)=="LAMPARA2=ON")//Si a la posición 'posicion2' hay "VENTILADOR=ON"
          {
            digitalWrite(lampara2,HIGH);
            estado2="ON";
          }
          if(cadena.substring(posicion2)=="LAMPARA2=OFF")//Si a la posición 'posicion2' hay "VENTILADOR=OFF"
          {
            digitalWrite(lampara2,LOW);
            estado2="OFF";
          }
 
  
        //Cuando reciba una línea en blanco, quiere decir que la petición HTTP ha acabado y el servidor Web está listo para enviar una respuesta
        if (c == '\n' && currentLineIsBlank) {
 
            // Enviamos al cliente una respuesta HTTP
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
 
            //Página web en formato HTML
            client.println("<html>");
            client.println("<head>");
            client.print("<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>");

            client.println("</head>");
            client.println("<body>");
            client.print("<h2 class='text-muted text-center mt-5 mb-5'>Primeros pasos con Internet Of Things</h2>");
            //Creamos los botones. Para enviar parametros a través de HTML se utiliza el metodo URL encode. Los parámetros se envian a través del símbolo '?'
            client.println("<div style='text-align:center;'>");
            client.println("<button onClick=location.href='./?LAMPARA=ON\' class='btn btn-outline-success'>");
            client.println("ON");
            client.println("</button>");
            client.println("<button onClick=location.href='./?LAMPARA=OFF\' class='btn btn-outline-danger'>");
            client.println("OFF");
            client.println("</button>");
            client.println("<br /><br />");
            client.println("<b>LAMPARA = ");
            client.print(estado1);
            client.println("</b><br />");
            client.println("</b><br />");
            client.println("<button onClick=location.href='./?LAMPARA2=ON\' class='btn btn-outline-success'>");
            client.println("ON");
            client.println("</button>");
            client.println("<button onClick=location.href='./?LAMPARA2=OFF\' class='btn btn-outline-danger'>");
            client.println("OFF");
            client.println("</button>");
            client.println("<br /><br />");
            client.println("<b>LAMPARA2 = ");
            client.print(estado2);
            client.println("</b><br />");
            client.println("</b></body>");
            client.println("</html>");
            break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    //Dar tiempo al navegador para recibir los datos
    delay(1);
    client.stop();// Cierra la conexión
  }
}
