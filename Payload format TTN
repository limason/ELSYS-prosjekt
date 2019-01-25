function Decoder(bytes, port) {
    //Definerer de ulike elementene i bufferen
    var temp = ((bytes[0] << 8) + bytes[1]);
    var turb = bytes[2];
    
    //Returerer verdiene.
    return {
      temperatur: temp,
      turbiditet: turb
    };
}
