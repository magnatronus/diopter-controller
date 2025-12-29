'use strict';


// Get the query string part of the URL
const queryString = window.location.search;
showLeftHorizontalDiopter();
showLeftVerticalDiopter();

// what value does leftButtonVertical have
function readLeftButtonVertical(){
    var val = localStorage.getItem('luv');
    if(val == null){
        val = 0;
    }
    return val;
}

// what value does leftButtonHorizontal have
function readLeftButtonHorizontal(){
    var val = localStorage.getItem('luh');
    if(val == null){
        val = 0;
    }
    return val;
}

function showLeftHorizontalDiopter(){
    document.getElementById('leftHorizontalDiopter').innerHTML =
    'H: ' + readLeftButtonHorizontal() + ' diopter';
}

function showLeftVerticalDiopter(){
    document.getElementById('leftVerticalDiopter').innerHTML =
    'V: ' + readLeftButtonVertical() + ' diopter';
}

// left calibrate
document.querySelector("#btn-left-calibrate").addEventListener("click", () => {
    localStorage.setItem('luv', 0);
    localStorage.setItem('luh', 0);
    showLeftVerticalDiopter();
});

// left up
document.querySelector("#btn-left-up").addEventListener("click", () => {
    var val = parseInt(readLeftButtonVertical());
    val+=2;
    localStorage.setItem('luv', String(val));
    showLeftVerticalDiopter();
});

// left down
document.querySelector("#btn-left-down").addEventListener("click", () => {
    var val = parseInt(readLeftButtonVertical());
    val-=2;
    localStorage.setItem('luv', String(val));
    showLeftVerticalDiopter();
});

// left left
document.querySelector("#btn-left-left").addEventListener("click", () => {
    var val = parseInt(readLeftButtonHorizontal());
    val -= 2;
    localStorage.setItem('luh', String(val));
    showLeftHorizontalDiopter();
});

// left right
document.querySelector("#btn-left-right").addEventListener("click", () => {
    var val = parseInt(readLeftButtonHorizontal());
    val+=2;
    localStorage.setItem('luh', String(val));
    showLeftHorizontalDiopter();
});



