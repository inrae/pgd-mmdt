function sendData(type=1, _field_='title', _sortby_=1)
{
	var XHR = new XMLHttpRequest();

	// Define what happens if the submission is successful
	var divcontainer = document.getElementById("container");
	XHR.addEventListener("load", function(event) {
		divcontainer.innerHTML= event.target.responseText;
	});

	// Define what happens if the submission is failed
	XHR.addEventListener("error", function(event) {
		alert('Oops! Something went wrong. Try again.');
	});

	// Request
	XHR.open("POST", "view", true); // URL relative OK
	XHR.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");

	// Simple form 
	if (type==2) {
		search2 = $.trim($("#search2").val());
		operator = $('input[name="operator2"]:checked').val();
		if (search2.length) {
			if (DEBUG) console.log('keywords: '+search2+', operator: '+operator)
			XHR.send('query='+encodeURIComponent(search2)+'&operator='+operator);
		} else {
			if (DEBUG) console.log('Reset formsearch => type = 1')
			document.getElementById("formsearch").reset();
			type=1;
		}
	}

	// Advanced form : Link the FormData object and the form element then transform Formdata into json for passing to PHP
	if (type==1) {
		var form = document.getElementById("formsearch");
		var FD = new FormData(form);
		var object = {};
		FD.forEach((value, key) => {object[key] = value});
		object['_field_'] = _field_;
		object['_sortby_'] = _sortby_;
		var json = JSON.stringify(object);	
		if (DEBUG) console.log(json)
		XHR.send('param='+json);
	}

}

submitForm1 = function () {
	if (!$('input[name=operator]:checked').val()) {
		alert('You must indicate whether your search fields are mandatory or optional!');
	} else {
		sendData(type=1);
	}
}

submitForm2 = function () {
	if (!$('input[name=operator2]:checked').val()) {
		alert('You must indicate whether your search fields are mandatory or optional!');
	} else {
		sendData(type=2);
	}
}

window.addEventListener("load", function ()
{
	// Support the submit event from the Advanced form
	var form = document.getElementById("formsearch");
	form.addEventListener("submit", function (event) {
		event.preventDefault();
		if (DEBUG) console.log('Submit Form1')
		submitForm1()
	});

	// Support the submit event from the Simple form
	var form2 = document.getElementById("formsearch2");
	form2.addEventListener("submit", function (event) {
		event.preventDefault();
		submitForm2()
	});

});
