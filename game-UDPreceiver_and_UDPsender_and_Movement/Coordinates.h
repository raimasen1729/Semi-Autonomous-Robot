/*
 * HTML and Javascript code
 */
const char body[] PROGMEM = R"===(
<!DOCTYPE html>
<html><body>

<button type="button1" onclick="hit1()" style="height:50px;width:50px;border-radius:25px; top: 0px; left: 200px; position: absolute; background-color: pink;"> Ok </button>
   <span id="somelabel1">  </span>  <br> 

<input type="range" min="2000" max="6000" value="4000" oninput="this.nextElementSibling.value = this.value" id="slider1" style="top: 100px; left: 50px; position: absolute;">
<output style="top: 402px; left: 200px; position: absolute;">50</output> 
<span id="outputlabel1">  </span> <br>

<input type="range" min="2000" max="6000" value="4000" oninput="this.nextElementSibling.value = this.value" id="slider2" style="top: 0px; left: 50px; position: absolute;">
<output style="top: 302px; left: 200px; position: absolute;">50</output> 
<span id="outputlabel2">  </span> <br>

</body>

<script>
function hit1() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "hit1", true);
      xhttp.send();
   } 

//Function for sending xml request on encountering the event of change of speeds
slider1.onchange = function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("outputlabel1").innerHTML = this.responseText;
    }
  };
  var str1 = "slider?val1=";
  var res = str1.concat(this.value);
  xhttp.open("GET", res, true);
  xhttp.send();
}

slider2.onchange = function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("outputlabel2").innerHTML = this.responseText;
    }
  };
  var str2 = "slider?val2=";
  var res = str2.concat(this.value);
  xhttp.open("GET", res, true);
  xhttp.send();
}

</script>

</html>
)===";
