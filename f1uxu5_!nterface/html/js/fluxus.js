var websock;

window.onload = function() {
    document.getElementById("alpha").oninput = onInputAlpha;
    document.getElementById("offX").oninput = onInputOffX;
    document.getElementById("offY").oninput = onInputOffY;
    document.getElementById("reps").oninput = onInputReps;
    document.getElementById("flipH").oninput = onInputFlipH;
    document.getElementById("flipV").oninput = onInputFlipV;

    websock = new WebSocket("ws://" + location.hostname + ":9092");

 //   websock.onopen = function (event) {
 //       console.log('yaay'); 
 //   };

}

function onInputAlpha() {
    let a = document.getElementById("alpha").value/1000.0;
    document.getElementById("v-alpha").innerHTML = "" + a;

    websock.send("aa:" + a);
}

function onInputOffX() {
    let a = document.getElementById("offX").value/100.0;
    document.getElementById("v-offX").innerHTML = "" + a;

    websock.send("ox:" + a);
}

function onInputOffY() {
    let a = document.getElementById("offY").value/100.0;
    document.getElementById("v-offY").innerHTML = "" + a;

    websock.send("oy:" + a);
}

function onInputReps() {
    let a = document.getElementById("reps").value;
    document.getElementById("v-reps").innerHTML = "" + a;

    websock.send("rp:" + a);
}

function onInputFlipH() {
    
    if (document.getElementById("flipH").checked) {
        websock.send("fh:1");
    }
    else {
        websock.send("fh:0");
    }
}

function onInputFlipV() {
    
    if (document.getElementById("flipV").checked) {
        websock.send("fv:1");
    }
    else {
        websock.send("fv:0");
    }
}

