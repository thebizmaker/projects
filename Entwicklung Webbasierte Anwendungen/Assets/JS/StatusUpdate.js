/*jslint browser */
//"use strict";
/*global console, XMLHttpRequest, window, document*/

var request = new XMLHttpRequest();

window.onload = requestData;

// Close the window and clear the session storage
// window.close = function(){
//     window.sessionStorage.clear();
// }

function process($data) {
    let data = JSON.parse($data);
    // console.log(data);
    const wraper = document.getElementsByClassName("wraper")[0];
    wraper.textContent = "";
    if (data === null || data.length === 0) {
        wraper.textContent = "Es wurde noch keine Bestellung getätigt !";
        document.getElementsByClassName("div-gpreis-status")[0].remove();
        return;
    }
    let totalPrice = 0;
    Array.from(data).forEach(function (element) {
        let status = parseInt(element.status);
        let ordered_id = element.ordered_id;
        let picture = element.picture;
        let name = element.name;
        let price = element.price;
        let item = document.createElement("div");
        let img = document.createElement("img");
        let itemName = document.createElement("p");
        let newStatus = document.createElement("p");
        img.src = picture;
        img.alt = name;
        img.style.width = "200px";
        img.style.height = "200px";
        item.appendChild(img);
        itemName.textContent = name + " " + price + "€";
        item.appendChild(itemName);
        let divStatus = document.createElement("div");
        divStatus.classList.add("div-gpreis");
        switch (status) {
            case 0:
                newStatus.textContent = "Bestellt";
                newStatus.style.color = "#17a2b8";
                break;
            case 1:
                newStatus.textContent = "Im Ofen";
                newStatus.style.color = "#ffc107";
                break;
            case 2:
                newStatus.textContent = "Fertig";
                newStatus.style.color = "#28a745";
                break;
            case 3:
                newStatus.textContent = "Unterwegs";
                newStatus.style.color = "#007bff";
                break;
            case 4:
                newStatus.textContent = "Ausgeliefert";
                newStatus.style.color = "#343a40";
                break;
        }
        newStatus.id = ordered_id;
        divStatus.appendChild(newStatus);
        item.appendChild(divStatus);
        wraper.appendChild(item);
        totalPrice += parseFloat(price);
    });
    let div_preis = document.getElementsByClassName("div-gpreis-status")[0];
    const gesamtpreis = document.getElementById("totalprice");
    if (div_preis === undefined || div_preis === null) {
        const divGpreis = document.createElement("div");
        divGpreis.classList.add("div-gpreis-status");
        gesamtpreis.textContent = `Gesamtpreis: ${totalPrice.toFixed(2)} €`;
        divGpreis.appendChild(gesamtpreis);
        wraper.appendChild(divGpreis);
    } else {
        gesamtpreis.textContent = `Gesamtpreis: ${totalPrice.toFixed(2)} €`;
    }
}

function requestData() {
    request.open("GET", "KundenStatus.php");
    request.onreadystatechange = processData;
    request.send(null);
}

function processData() {
    if (request.readyState === 4) {
        if(request.status === 200){
            if (request.responseText !== null) {
                process(request.responseText);
            } else {
                console.error("Dokument is empty !");
            }
        } else {
            console.error("Transmission failed !");
        }
    }
}

window.setInterval(requestData, 2000);