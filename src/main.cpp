#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <WiFiMulti.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include "time.h"
#include "FS.h"
#include <SPI.h>
#include <SD.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
WiFiMulti wifiMulti;
AsyncWebServer server(83);
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600 * 2;
const int daylightOffset_sec = 3600 * 0;
// configTzTime("CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", ntpServer);
#define SD_MOSI 23
#define SD_MISO 19
#define SD_SCLK 18
int alimentationSonde = 32;
int masseSonde = 33;
int entreeSonde = 35;
const byte SD_CS = 5;
int monMois = 0;
int monAnnee = 0;
long millisHeure = 0;
int taille_fichier;
String chaineAnnee;
String chaineHeure;
String chaineMois;
String moisDuJour;
String chaineJour;
String chaineMinute;
String chaineSeconde;
String dernier_evenement = "";
String monDernierEvenement = "";
String monPremierEvenement = "";
String premierEvenement = "";
String monDernierJour = "";
String monDernierJourHtml = "";
String calculEvenement = "";
String cheminFichierJour = "";
char character;
int nb_de_donnees = 0;
int nouvelleMesure = 0;
int positionlecture;
int nombreFichesJour;
int moisEnCours;
const int ledVerte = 12;
const int ledBleueWifi = 14;
const int ledRougeWifi = 27;
char message[20];
char aujourdhui[10];
char maintenant[30];
char mahauteur[10]; // Allocate a character array to store the result
char maHauteurJour[10];
char donneesJour[30];
char maHauteurJourHtml[30];
char monhorloge[30];
String maHauteurTotale = "";
String maHauteurTotaleJour = "";
String mesMois[] = {"", "janvier", "février", "mars", "avril", "mai", "juin", "juillet", "août", "septembre", "octobre", "novembre", "décembre"};
// String mesJours[] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};
int mavar = 0;
boolean lectureHeureDebut = false;
const int stockage_max = 10;
int mesures[stockage_max];
time_t date_et_heure[stockage_max];
int pulsion = 13;
String ledState;
int nombre_enregistrements;
float hauteur;
float hauteurJour;
String hauteurChaine;
float volume_impulsion = 0.375;
File monfichier;
File monfichierJour;

void lecturefichierDuJour()
{
  Serial.print("lecture fichier jour s'il existe ");
  Serial.println(monDernierJour);
  // Serial.print("cheminFichierJour    d  ns lecture       ");
  Serial.println(cheminFichierJour);
  if (SD.exists(cheminFichierJour))
  {
    Serial.println("fichier trouvé");
    Serial.print("monDernierJour : ");
    Serial.println(monDernierJour);
    Serial.print("cheminFichierJour : ");
    Serial.println(cheminFichierJour);
    monfichierJour = SD.open(cheminFichierJour, FILE_READ);
    taille_fichier = monfichierJour.size();
    Serial.print("taille fichier : ");
    Serial.println(taille_fichier);
    nombreFichesJour = taille_fichier / 18;
    nombreFichesJour++;
    monfichierJour.close();
    Serial.print("nombre_enregistrements du jour : ");
    Serial.println(nombreFichesJour);
    hauteurJour = nombreFichesJour * volume_impulsion;
    Serial.print("hauteurJour   ");
    Serial.println(hauteurJour);
    sprintf(maHauteurJourHtml, "Aujourd'hui : %6.1lf mm", hauteurJour);
    dtostrf(hauteurJour, 6, 2, maHauteurJour);
    maHauteurTotaleJour = maHauteurJour;
    Serial.print("maHauteurTotaleJour   ");
  }
  else
  {
    Serial.println("fichier inexistant");
    sprintf(maHauteurJourHtml, "Pas de pluie aujourd'hui %s", "");
  }
  // delay(2000);
}
// mise en forme enregistrements
void miseEnForme()
{
  Serial.print("miseEnForme suite ");
  monDernierEvenement = "";
  monDernierEvenement = monDernierEvenement + calculEvenement.charAt(6);
  monDernierEvenement = monDernierEvenement + calculEvenement.charAt(7);
  monDernierEvenement = monDernierEvenement + ("/");
  monDernierEvenement = monDernierEvenement + calculEvenement.charAt(4);
  monDernierEvenement = monDernierEvenement + calculEvenement.charAt(5);
  monDernierEvenement = monDernierEvenement + ("/");
  monDernierEvenement = monDernierEvenement + calculEvenement.charAt(0);
  monDernierEvenement = monDernierEvenement + calculEvenement.charAt(1);
  monDernierEvenement = monDernierEvenement + calculEvenement.charAt(2);
  monDernierEvenement = monDernierEvenement + calculEvenement.charAt(3);
  monDernierEvenement = monDernierEvenement + (" à ");
  monDernierEvenement = monDernierEvenement + calculEvenement.charAt(9);
  monDernierEvenement = monDernierEvenement + calculEvenement.charAt(10);
  monDernierEvenement = monDernierEvenement + ("h");
  monDernierEvenement = monDernierEvenement + calculEvenement.charAt(11);
  monDernierEvenement = monDernierEvenement + calculEvenement.charAt(12);
  monDernierEvenement = monDernierEvenement + ("mn");
  monDernierEvenement = monDernierEvenement + calculEvenement.charAt(13);
  monDernierEvenement = monDernierEvenement + calculEvenement.charAt(14);
  monDernierEvenement = monDernierEvenement + ("s");
  Serial.println(monDernierEvenement);
  monDernierJour = "";
  calculEvenement = aujourdhui;
  Serial.println(aujourdhui);
  // Serial.println(calculEvenement);
  monDernierJour = monDernierJour + calculEvenement.charAt(0);
  monDernierJour = monDernierJour + calculEvenement.charAt(1);
  monDernierJour = monDernierJour + calculEvenement.charAt(2);
  monDernierJour = monDernierJour + calculEvenement.charAt(3);
  monDernierJour = monDernierJour + calculEvenement.charAt(4);
  monDernierJour = monDernierJour + calculEvenement.charAt(5);
  monDernierJour = monDernierJour + calculEvenement.charAt(6);
  monDernierJour = monDernierJour + calculEvenement.charAt(7);
  Serial.print("monDernierJour            ");
  Serial.println(monDernierJour);
  cheminFichierJour = "/pluiejours/" + monDernierJour + ".txt";
  Serial.print("cheminFichierJour ");
  Serial.println(cheminFichierJour);
}

// void lecturepremier_sd()
// {
//   calculEvenement = "";
//   Serial.println("lecture premier enregistrement du fichier pluie.");
//   monfichier = SD.open("/PLUIE.txt ", FILE_READ);
//   monfichier.seek(17);
//   for (int i = 0; i <= 17; i++)
//   {
//     (character = monfichier.read());
//     delay(100);
//     // Serial.print(character);
//     calculEvenement = calculEvenement + character;
//   }
//   monfichier.close();
// }

void lecturedernier_sd()
{
  calculEvenement = "";
  Serial.println("lecture dernier enregistrement du fichier pluie.");
  monfichier = SD.open("/PLUIE.txt ", FILE_READ);
  taille_fichier = monfichier.size();
  Serial.print("taille fichier : ");
  Serial.println(taille_fichier);
  nombre_enregistrements = taille_fichier / 18;
  // nombre_enregistrements = nombre_enregistrements + 1;
  Serial.print("nombre_enregistrements : ");
  Serial.println(nombre_enregistrements);
  delay(100);
  positionlecture = taille_fichier - 18; // dernier caractere du fichier
  Serial.print("positionlecture fichier : ");
  Serial.println(positionlecture);
  monfichier.seek(positionlecture + 1);
  for (int i = 0; i <= 17; i++)
  {
    (character = monfichier.read());
    delay(100);
    Serial.print(character);
    dernier_evenement = dernier_evenement + character;
  }
  calculEvenement = dernier_evenement;
  Serial.print("calculEvenement dans lecture du dernier ");
  Serial.println(calculEvenement);
  monfichier.close();
  miseEnForme();
}

void conversionVolume()
{
  dtostrf(hauteur, 6, 1, mahauteur);
  Serial.print("ma hauteur en chaine ");
  Serial.println(mahauteur);
  maHauteurTotale = "Cumul année: " + maHauteurTotale + " mm";
  maHauteurTotale = mahauteur;
  Serial.println(maHauteurTotale);
}

void demarrageWifi()
{
  wifiMulti.addAP("HubertBricau", "ccAk688b2");
  wifiMulti.addAP("HubertBricau_Ext", "ccAk688b2");
  wifiMulti.addAP("Bbox-8A640A05", "ccAk688b2");
  IPAddress ip(192, 168, 1, 8);
  IPAddress gateway(192, 168, 1, 254);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress dns(192, 168, 1, 254);
  WiFi.config(ip, gateway, subnet, dns);
  Serial.println("\n");
  Serial.println("Attente connexion");
  if (wifiMulti.run() != WL_CONNECTED)
  {
    Serial.println("WiFi not connected!");
    delay(1000);
  }
  Serial.println("\n");
  Serial.println("Connexion établie .... ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Ssid: ");
  Serial.println(WiFi.SSID());
  Serial.print("Niveau réception: ");
  Serial.println(WiFi.RSSI());
  Serial.println("");
  digitalWrite(ledRougeWifi, HIGH);
}
// ====================================================

void appendFile(fs::FS &fs, const char *path, const char *message)
{
  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("Echec de l'ouverture du fichier");
    return;
  }
  if (file.print(message))
  {
    Serial.println("Modification du fichier reussie.");
  }
  else
  {
    Serial.println("Echec de la modification du fichier");
  }
  file.close();
}

void gestionHeure()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
  }
  // Serial.print("heure setup : ");
  // Serial.println(&timeinfo, "%d%m%Y %H:%M:%S");
  {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    monAnnee = timeinfo->tm_year + 1900;
    chaineAnnee = monAnnee;
    int monAnnee = timeinfo->tm_year + 1900;
    chaineAnnee = monAnnee;
    monMois = timeinfo->tm_mon + 1;
    if (monMois < 10) // gestion avant 10
    {
      chaineMois = "0";
      chaineMois += monMois;
    }
    else
    {
      chaineMois = monMois;
    }
    //===============================================
    if (timeinfo->tm_mday < 10) // gestion avant 10
    {
      chaineJour = "0";
      chaineJour += timeinfo->tm_mday;
    }
    else
    {
      chaineJour = timeinfo->tm_mday;
    }
    //===============================================
    if (timeinfo->tm_hour < 10) // gestion avant 10
    {
      chaineHeure = "0";
      chaineHeure += timeinfo->tm_hour;
    }
    else
    {
      chaineHeure = timeinfo->tm_hour;
    }
    //===============================================
    if (timeinfo->tm_min < 10) // gestion avant 10
    {
      chaineMinute = "0";
      chaineMinute += timeinfo->tm_min;
    }
    else
    {
      chaineMinute = timeinfo->tm_min;
    }
    //===============================================
    if (timeinfo->tm_sec < 10) // gestion avant 10
    {
      chaineSeconde = "0";
      chaineSeconde += timeinfo->tm_sec;
    }
    else
    {
      chaineSeconde = timeinfo->tm_sec;
    }
    sprintf(maintenant, "%s/%s/%s à  %sh%smn%ss", chaineJour, chaineMois, chaineAnnee, chaineHeure, chaineMinute, chaineSeconde);
    sprintf(aujourdhui, "%s%s%s", chaineJour, chaineMois, chaineAnnee);
    // Serial.print("aujourd'hui    _______________");
    // Serial.println(aujourdhui);
    // Serial.print("maintenant    _______________");
    // Serial.println(maintenant);
    sprintf(message, "%s%s%s;%s%s%s;\n", chaineAnnee, chaineMois, chaineJour,
            chaineHeure, chaineMinute, chaineSeconde);

    // Stockage des 10 mesures les plus rÃ©centes pour affichage dans la page web
    // if (nb_de_donnees < stockage_max)
    // { // il y a encore des lignes vides pour l'affichage web
    //   mesures[nb_de_donnees] = nouvelleMesure;
    //   date_et_heure[nb_de_donnees] = heureMesure; // quelle heure est-il?
    //   nb_de_donnees++;
    // }
    // else // la page web est pleine
    // {
    //   for (int i = 0; i < stockage_max - 1; i++)
    //   { // on dÃ©cale toutes les infos d'une ligne
    //     mesures[i] = mesures[i + 1];
    //     date_et_heure[i] = date_et_heure[i + 1];
    //   }
    //   mesures[stockage_max - 1] = nouvelleMesure;    // on enregistre la nouvelle info
    //   date_et_heure[stockage_max - 1] = heureMesure; // quelle heure est-il?
    // }
    hauteur = nombre_enregistrements * volume_impulsion;
    // Serial.println(mahauteur);
  }
}

void nombreImpulsions() // affichage au demarrage
{
  Serial.println("Ouverture du fichier pluie.");
  monfichier = SD.open("/PLUIE.txt", FILE_READ);
  Serial.print("taille fichier : ");
  Serial.println(monfichier.size());
  nombre_enregistrements = monfichier.size() / 18;
  Serial.print("nombre enregistrements :");
  Serial.println(nombre_enregistrements);
  delay(100);
  hauteur = nombre_enregistrements * volume_impulsion;
  dtostrf(hauteur, 6, 1, mahauteur);
  maHauteurTotale = mahauteur;
  maHauteurTotale = "Cumul année: " + maHauteurTotale + " mm";
  monfichier.close();
  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);
  display.print(nombre_enregistrements);
  display.println(" impulsions");
  display.print(hauteur, 1);
  display.print(" mm");
  display.display();
}

// Replaces placeholder with LED state value
String processor(const String &var)
{
  if (var == "STATE")
  {
    return ledState;
  }

  return String();
}
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù

void setup()
{
  pinMode(ledBleueWifi, OUTPUT);
  digitalWrite(ledBleueWifi, LOW);
  pinMode(pulsion, INPUT_PULLUP);
  pinMode(ledRougeWifi, OUTPUT);
  digitalWrite(ledRougeWifi, LOW);
  Serial.begin(115200);
  pinMode(ledVerte, OUTPUT);
  digitalWrite(ledVerte, LOW);
  pinMode(alimentationSonde, OUTPUT);
  pinMode(masseSonde, OUTPUT);
  pinMode(entreeSonde, INPUT);
  demarrageWifi();
  Serial.println("OLED intialized");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  display.display();
  display.clearDisplay();
  // display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Gestion pluie");
  display.display();
  delay(2000);
  sprintf(maHauteurJourHtml, "Pas de pluie aujourd'hui", hauteurJour);
  digitalWrite(alimentationSonde, HIGH);
  digitalWrite(masseSonde, LOW);
  SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS))
  {
    Serial.println("SD Card MOUNT FAIL");
  }
  else
  {
    Serial.println("SD Card MOUNT SUCCESS");
    Serial.println("");
    uint32_t cardSize = SD.cardSize() / (1024 * 1024);
    String str = "Carte: " + String(cardSize) + " MB";
    Serial.println(str);
    Serial.print("Byte utilisés : ");
    Serial.println(SD.usedBytes());
    // display.clearDisplay();
    // display.setCursor(0, 0);
    // display.print(str);
    // display.setCursor(0, 1);
    // display.print("en ligne");
    delay(3000);
  }

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  gestionHeure();
  lecturedernier_sd();
  lecturefichierDuJour(); // voir si fichier impulsions dans la journée existe
  Serial.println(maHauteurJourHtml);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  Serial.println(WiFi.localIP());
  // transmission des donnée ves java et appel  fonctions
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", String(), false, processor); });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/script.js", "text/javascript"); });
  // mise en forme données pour affichage
  server.on("/lireMonDernier", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", monDernierEvenement); });
  server.on("/lireMonPremier", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", monPremierEvenement); });
  server.on("/hauteurTotale", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", maHauteurTotale); });
  server.on("/mescalculs", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", maHauteurJourHtml); });

  // Start server
  server.begin();
  // On configure le seveur NTP
  nombreImpulsions();
  hauteur = nombre_enregistrements * volume_impulsion;
  Serial.print("\n");
  Serial.print("hauteur : ");
  Serial.println(hauteur);
  Serial.println(monDernierJour);
}
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
void loop()
{
  gestionHeure();

  if( chaineSeconde == "00" && chaineMinute == "00" && chaineHeure == "00"){
  Serial.println(chaineMinute);
  Serial.println(chaineSeconde);
  lecturefichierDuJour();
  delay(1000);
  }
  if (WiFi.status() != 3)
  {
    digitalWrite(ledVerte, LOW);
    digitalWrite(ledRougeWifi, HIGH);
    demarrageWifi();
  }
  else
  {
    digitalWrite(ledRougeWifi, LOW);
    digitalWrite(ledVerte, HIGH);
  }
  mavar = digitalRead(pulsion);
  if (mavar == 0) // entrée d'une impulsion
  {
    delay(500);
    Serial.println(pulsion);
    hauteur = hauteur + volume_impulsion;
    Serial.println("hauteur apres impulsion");
    Serial.println(hauteur);
    conversionVolume();
    Serial.println("");
    gestionHeure();
    calculEvenement = message;
    appendFile(SD, "/PLUIE.txt", message);
    nombreImpulsions();
    lecturedernier_sd();
    monDernierEvenement = maintenant;
    sprintf(donneesJour, "/pluiejours/%s%s%s%s\n", chaineJour, chaineMois, chaineAnnee, ".txt");
    Serial.print(donneesJour);
    Serial.print("données du jour apres impulsion : ");
    appendFile(SD, donneesJour, message);
    Serial.print("hauteurJour   ");
    Serial.println(hauteurJour);
    hauteurJour = hauteurJour + volume_impulsion;
    dtostrf(hauteurJour, 6, 2, maHauteurJour);
    maHauteurTotaleJour = maHauteurJour;
    sprintf(maHauteurJourHtml, "Aujourd'hui : %6.1lf mm", hauteurJour);
    Serial.println(maHauteurJourHtml);
  }
}
