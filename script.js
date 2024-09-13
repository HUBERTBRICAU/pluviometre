let mesmois = ['janvier','février','mars','avril','mai','juin','juillet','août',
'septembre','octobre','novembre','decembre']
let  mesJours= ["Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"];
let currentTime = new Date();
let xhttp0 = new XMLHttpRequest();
let xhttp = new XMLHttpRequest();
let xhttp1 = new XMLHttpRequest();
let xhttp2 = new XMLHttpRequest();
let xhttp3 = new XMLHttpRequest();
let dateHeure = "";
let jour = "";
let mois = "";

setInterval(function getData() {
  xhttp1.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("derniereEntree").innerHTML = this.responseText;
    }
  }
  xhttp1.open("GET", "lireMonDernier", true);
  xhttp1.send();
  //=====================================================
  {
    xhttp2.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("premiereEntree").innerHTML = this.responseText;
      }
    }
    xhttp2.open("GET", "lireMonPremier", true);
    xhttp2.send();
  }
  // =====================================================
  {
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("hauteurCumulee").innerHTML = this.responseText;
      }
    }
    xhttp.open("GET", "hauteurTotale", true);
    xhttp.send();
  }
  // =====================================================
    {
    xhttp0.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("resultatsMensuels").innerHTML = this.responseText;
      }
    }
    xhttp0.open("GET", "mescalculs", true);
    xhttp0.send();
  }
  {
    // affichage date
      currentTime = new Date();
      dateHeure = currentTime.getDay() + " "+ currentTime.toLocaleTimeString();
      console.log(dateHeure);
      jour = mesJours[currentTime.getDay()];
      mois = mesmois[currentTime.getMonth()];
      document.getElementById("afficheDate").innerHTML =jour+" "+currentTime.getDate()
      +" "+mois+" "+ currentTime.getFullYear()+"  " +currentTime.toLocaleTimeString('it-IT')+" "
      ;
      // console.log(event.toLocaleTimeString('it-IT'));
  }
},1000);
  
