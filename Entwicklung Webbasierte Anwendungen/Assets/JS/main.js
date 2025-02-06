/*jslint browser */
//"use strict";
/*global document*/

const adresse = document.getElementById("adresse");
const button1 = document.getElementById("button1");
const button2 = document.getElementById("button2");
const button3 = document.getElementById("button3");
const items = document.getElementsByTagName("img");
const warenkorb = document.getElementById("warenkorb");
const gesamtpreis = document.getElementById("gesamtpreis");
const currrentPage = window.location.pathname.split("/").pop();


function addItem(name, price) {
    let optionText = "+ " + name + " " + price + "€";
    let option = document.createElement("option");
    option.value = name;
    option.text = optionText;
    warenkorb.appendChild(option);

    updatePrice();
    verifyForm();
}

Array.from(items).forEach(function (item) {
    if (currrentPage === "bestellung.php") {
        item.onclick = function () {
            let name = item.alt;
            let price = parseFloat(item.dataset.price);
            addItem(name, price);
        };
    }
});

function updatePrice() {
    if (warenkorb.options.length === 0) {
        gesamtpreis.textContent = "Gesamtpreis: 0 €";
        return;
    }
    let totalPrice = 0;
    Array.from(warenkorb.options).forEach(function (option) {
        let price = option.textContent.split(" ")[2];
        price = price.substring(0, price.length - 1);
        totalPrice += parseFloat(price.trim());
    });
    gesamtpreis.textContent = `Gesamtpreis: ${totalPrice.toFixed(2)} €`;
}

button1.addEventListener("click", function (event) {
    event.preventDefault();
    while (warenkorb.options.length > 0) {
        warenkorb.remove(0);
    }
    gesamtpreis.textContent = "Gesamtpreis: 0 €";
    verifyForm();
});

button2.addEventListener("click", function (event) {
    event.preventDefault();
    Array.from(warenkorb.options).forEach(function (option) {
        if (option.selected) {
            warenkorb.remove(option.index);
        }
    });
    
    updatePrice();
    verifyForm();
});

button3.onclick = function () {
    selectAll();
    verifyForm();
};

adresse.oninput = function () {
    verifyForm();
};

function verifyForm() {
    if (adresse.value.trim() === "" || warenkorb.options.length === 0) {
        button3.disabled = true;
    } else {
        button3.disabled = false;
    }
}

function selectAll() {
    Array.from(warenkorb.options).forEach(function (option) {
        option.text = option.text.split(" ")[1].trim();
        option.selected = true;
    });
}

function orderDone() {}