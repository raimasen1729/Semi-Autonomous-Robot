/*
 * HTML and Javascript code
 */
const char body[] PROGMEM = R"===(
<!DOCTYPE html>
<html><body>

<button type="button1" onclick="hit1()" style="height:50px;width:50px;border-radius:25px; top: 0px; left: 0px; position: absolute; transform: rotate(45deg); background-color: pink;"> < </button>
   <span id="somelabel1">  </span>  <br> 
   
<button type="button2" onclick="hit2()" style="height:50px;width:50px;border-radius:25px; top: 0px; left: 100px; position: absolute; transform: rotate(90deg); background-color: pink;"> < </button>
   <span id="somelabel2">  </span>  <br> 
   
<button type="button3" onclick="hit3()" style="height:50px;width:50px;border-radius:25px; top: 0px; left: 200px; position: absolute; transform: rotate(-45deg); background-color: pink;"> > </button>
   <span id="somelabel3">  </span>  <br> 
   
   
<button type="button4" onclick="hit4()" style="height:50px;width:50px;border-radius:25px; top: 100px; left: 0px; position: absolute; background-color: pink;"> < </button>
   <span id="somelabel4">  </span>  <br> 

<button type="button5" onclick="hit5()" style="height:50px;width:50px;border-radius:25px; top: 100px; left: 100px; position: absolute; background-color: pink;"> STOP </button>
   <span id="somelabel5">  </span>  <br> 
   
<button type="button6" onclick="hit6()" style="height:50px;width:50px;border-radius:25px; top: 100px; left: 200px; position: absolute; background-color: pink;"> > </button>
   <span id="somelabel6">  </span>  <br> 
   
   
<button type="button7" onclick="hit7()" style="height:50px;width:50px;border-radius:25px; top: 200px; left: 0px; position: absolute; transform: rotate(-45deg); background-color: pink;"> < </button>
   <span id="somelabel7">  </span>  <br> 
   
<button type="button8" onclick="hit8()" style="height:50px;width:50px;border-radius:25px; top: 200px; left: 100px; position: absolute; transform: rotate(90deg); background-color: pink;"> > </button>
   <span id="somelabel8">  </span>  <br> 
   
<button type="button9" onclick="hit9()" style="height:50px;width:50px;border-radius:25px; top: 200px; left: 200px; position: absolute; transform: rotate(45deg); background-color: pink;"> > </button>
   <span id="somelabel9">  </span>  <br> 
   

<button type="button10" onclick="hit10()" style="height:50px;width:50px;border-radius:25px; top: 300px; left: 50px; position: absolute; background-color: pink;"> CW </button>
   <span id="somelabel10">  </span>  <br> 

<button type="button11" onclick="hit11()" style="height:50px;width:50px;border-radius:25px; top: 300px; left: 150px; position: absolute; background-color: pink;"> CCW </button>
   <span id="somelabel11">  </span>  <br> 
   

<button type="button12" onclick="hit12()" style="height:50px;width:50px;border-radius:25px; top: 450px; left: 0px; position: absolute; background-color: blue;"> Manual </button>
   <span id="somelabel12">  </span>  <br> 

<button type="button13" onclick="hit13()" style="height:50px;width:50px;border-radius:25px; top: 450px; left: 100px; position: absolute; background-color: blue;"> WallF </button>
   <span id="somelabel13">  </span>  <br> 

<button type="button14" onclick="hit14()" style="height:50px;width:50px;border-radius:25px; top: 450px; left: 200px; position: absolute; background-color: blue;"> IR </button>
   <span id="somelabel14">  </span>  <br> 
   
<input type="range" min="0" max="100" value="50" oninput="this.nextElementSibling.value = this.value" id="slider2" style="top: 400px; left: 50px; position: absolute;">
<output style="top: 402px; left: 225px; position: absolute;">50</output> 
<span id="outputlabel2">  </span> <br>

</body>

<script>
function hit1() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "hit1", true);
      xhttp.send();
   } 
   
   function hit2() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "hit2", true);
      xhttp.send();
   } 

   function hit3() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "hit3", true);
      xhttp.send();
   } 

   function hit4() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "hit4", true);
      xhttp.send();
   } 

   function hit5() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "hit5", true);
      xhttp.send();
   } 

   function hit6() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "hit6", true);
      xhttp.send();
   } 

   function hit7() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "hit7", true);
      xhttp.send();
   } 

   function hit8() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "hit8", true);
      xhttp.send();
   } 

   function hit9() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "hit9", true);
      xhttp.send();
   } 

   function hit10() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "hit10", true);
      xhttp.send();
   } 

   function hit11() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "hit11", true);
      xhttp.send();
   } 

   function hit12() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "hit12", true);
      xhttp.send();
   } 

   function hit13() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "hit13", true);
      xhttp.send();
   } 

   function hit14() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "hit14", true);
      xhttp.send();
   } 

//Function for sending xml request on encountering the event of change of speeds
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
