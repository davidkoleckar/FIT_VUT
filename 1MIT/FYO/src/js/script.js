var r = 10;
var nextX, nextY;
var ball = [36];


(function(){
    //nacist cely dokument pro pracis jednotlivymi elementy
    document.addEventListener("DOMContentLoaded", function(event) { 

		for (var i = 0; i <= 5; i++) {
			for (var j = 0; j <= 5; j++) {
			    ball[6*i+j] = {
			        x: 5*x_in.value*i+10,
			        y: 5*x_in.value*j+10,
			        draw: circle
			    };
			    var b = ball[6*i+j];
			    b.draw(b.x,b.y);
			}
		}

		var move = document.getElementById("move");
		var freq = document.getElementById("freq");
		var res = document.getElementById("result");

		move.onclick = e => {
		    nextX = +x_in.value || 0;
		    nextY = +x_in.value || 0;
		    anim();
		  	var f = (f_in.value || 0) * 1000000;
			var lambda = 2 * (x_in.value || 0)/1000;
			var v = f * lambda ;
		    res.innerHTML = "C = "+v.toString()+"m/s";
		}

		freq.onclick = function(e){
			var f = (f_in.value || 0) * 1000000;
			var lambda = 2 * (x_in.value || 0)/1000;
			var v = f * lambda ;
			res.innerHTML = "C = "+v.toString()+"m/s";
		}

    });//end DOMContentLoaded
})();

function anim() {
    var canvas = document.getElementById("canvas");
 	var ctx = canvas.getContext("2d");

    ctx.clearRect(0, 0, canvas.width, canvas.height);
    for (var i = 0; i <= 5; i++) {
	  for (var j = 0; j <= 5; j++) {
	  	var b = ball[6*i+j];
	  	b.x = nextX *5 * i+10;
		b.y = nextY *5 * j+10;
    	b.draw(b.x,b.y);
      }
	}
}

function circle(x,y) {
  var canvas = document.getElementById("canvas");
  var ctx = canvas.getContext("2d");
  ctx.fillStyle = "red";
  ctx.beginPath();
  ctx.arc(x, y, r, 0, Math.PI * 2, true);
  ctx.fill();
  ctx.closePath();
}

function updateGear(val) {
    document.getElementById('zubu').value=val; 
    calculateFizeau();
}

function updateFreq(val) {
    document.getElementById('f').value=val; 
    calculateFizeau();
}

function updateVzdalenost(val) {
    document.getElementById('vzdalenost').value=val; 
    calculateFizeau();
}

function calculateFizeau(){
	var res = document.getElementById("fizeauresult");
	var D = document.getElementById('vzdalenost').value;
	var N = document.getElementById('zubu').value;
	var f = document.getElementById('f').value;
	var c = 4 * D * N * f;
	res.innerHTML = "c = " + c.toString() + " m/s";
}


