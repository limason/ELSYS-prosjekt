function Converter(decoded, port) {
  
  var converted = decoded;
  
  //---Temperatur---
  converted.temperatur = (converted.temperatur/100) - 10;
  
  //---Turbiditet---
  //Bør se mer på dette, kanskje kalibrere sensoren. Returnerer kun spenninga nå
  //https://www.dfrobot.com/wiki/index.php/File:Relationship_diagram.jpg
  converted.turbiditet = (5.0/255) * converted.turbiditet;


  return converted;
}
