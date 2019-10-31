var websock;

window.onload = function() {
    document.getElementById("alpha").oninput = onInputAlpha;
    document.getElementById("offX").oninput = onInputOffX;
    document.getElementById("offY").oninput = onInputOffY;
    document.getElementById("reps").oninput = onInputReps;
    document.getElementById("flipH").oninput = onInputFlipH;
    document.getElementById("flipV").oninput = onInputFlipV;

    document.getElementById("r-alpha").onclick = onResetAlpha;
    document.getElementById("r-offX").onclick = onResetOffX;
    document.getElementById("r-offY").onclick = onResetOffY;
    document.getElementById("r-reps").onclick = onResetReps;

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

function onResetAlpha() {
    document.getElementById("alpha").value = 950;
    onInputAlpha();
}


function onInputOffX() {
    let a = document.getElementById("offX").value/100.0;
    document.getElementById("v-offX").innerHTML = "" + a;

    websock.send("ox:" + a);
}

function onResetOffX() {
    document.getElementById("offX").value = 0;
    onInputOffX();
}


function onInputOffY() {
    let a = document.getElementById("offY").value/100.0;
    document.getElementById("v-offY").innerHTML = "" + a;

    websock.send("oy:" + a);
}

function onResetOffY() {
    document.getElementById("offY").value = 0;
    onInputOffY();
}


function onInputReps() {
    let a = document.getElementById("reps").value;
    document.getElementById("v-reps").innerHTML = "" + a;

    websock.send("rp:" + a);
}

function onResetReps() {
    document.getElementById("reps").value = 1;
    onInputReps();
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


