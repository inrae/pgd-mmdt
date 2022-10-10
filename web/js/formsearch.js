window.addEventListener("load", function () {

  // Accéder à l'élément form
  var form = document.getElementById("formsearch");

  // et prendre en charge l'événement submit
  form.addEventListener("submit", function (event) {
    event.preventDefault();
    if (!$('input[name=operator]:checked').val()) {
        alert('You must indicate whether your search fields are mandatory or optional!');
    } else {
        sendData();
    }
  });

   function sendData() {
    var XHR = new XMLHttpRequest();

    // Lier l'objet FormData et l'élément form puis transformer Formdata en json pour passage à PHP
    var FD = new FormData(form);
    var object = {};
    FD.forEach((value, key) => {object[key] = value});
    var json = JSON.stringify(object);

    // Définir ce qui se passe si la soumission s'est opérée avec succès
    var divcontainer = document.getElementById("container");
    XHR.addEventListener("load", function(event) {
      divcontainer.innerHTML= event.target.responseText;
      window.location = (""+window.location).replace(/#[A-Za-z0-9_]*$/,'')+"#results"
    });

    // Definir ce qui se passe en cas d'erreur
    XHR.addEventListener("error", function(event) {
      alert('Oops! Something went wrong. Try again.');
    });

    // Configuration de la requête //
    XHR.open("POST", "view", true); // URL relative OK

    XHR.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");

    // Les données envoyées sont ce que l'utilisateur a mis dans le formulaire
    XHR.send('param='+json);
  }

});
