function load_file(file)
{
	if (file.length <= 0) {
		alert("Please select a file in JSON format !");
		return false;
	}

	var fr = new FileReader(); // Reader
	fr.onload = function(e) {
		load_data(this.result)
	}
	fr.readAsText(file.item(0));
};

// Load a user's json
function load_data(json)
{
	var id = "";
	var tags = Object.keys(checkboite);
	var lists = Object.keys(listboite);
	var multisel = Object.keys(multiboite);
	var autotxt = Object.keys(txtboite);
	var datetxt = Object.keys(dateboite);
	var autoarea = Object.keys(areaboite);
	var textarea = autotxt.concat(autoarea).concat(datetxt);

	// Validation based on JSON schema
	indata = Base64.encode(json);
	resp='';
	if (DEBUG) console.log("Check if the JSON file is valid:");
	$.ajaxSetup({async:false});
	$.get( "check", { json: indata } )
			.done(function(outdata) { resp=JSON.parse(outdata); })
			.fail(function(outdata) { console.log(outdata); });
	$.ajaxSetup({async:true});
	if (DEBUG) console.log(resp);
	// If no errors
	if (resp.errors && resp.errors.length==0) {
	//ok=1; if (ok==1) {
		var result = JSON.parse(json);
		if (DEBUG) console.log(result);
		allItems = []; checkItems = []; multiItems=[]; listItems=[]; textItems=[];
		// For each item in the json: assignment of values in the form 
		$.each(result, function(item, obj) {
			for (var prop in obj) {
				// CheckBoxes
				if (tags.includes(item)) {
					id = item+`${obj[prop]}`.replace(/ /g,"_");
					id_info = item+` = ${obj[prop]}`;
					// Test if the box is present or not in the form
					if (document.getElementById(id)) {
						document.getElementById(id).checked=true;
						if (! checkItems.includes(item)) checkItems.push(item);
					} else {
						alert('Please re-enter this value in the corresponding text field of the form : '+id_info);
					}
				}
				// Multi-Select
				if (multisel.includes(item)) {
					var element = document.getElementById(item+'-sel')
					if (element != null) element.value = obj.join(', ');
					if (! multiItems.includes(item)) multiItems.push(item);
				}
				// Lists
				if (lists.includes(item)) {
					var element = document.getElementById(item+'_select')
					if (element != null) element.value = obj;
					if (! listItems.includes(item)) listItems.push(item);
				}
				// Text
				if (textarea.includes(item)) {
					document.getElementById(item).value = obj;
					if (! textItems.includes(item)) textItems.push(item);
				}
			}
		});
		allItems['checkbox']=checkItems; allItems['multi-select']=multiItems; allItems['dropbox']=listItems; allItems['textbox']=textItems;
		if (DEBUG) console.log(allItems)

		// Resources
		res=result['resources'];
		if (DEBUG) console.log(res)
		for( var i = 1; i <= res.length; ++i ) {
			if(i>1) add_bouton_r(i);
			document.getElementsByName('resource-f1-'+i)[0].value=res[i-1]['datatype'];
			document.getElementsByName('resource-f2-'+i)[0].value=res[i-1]['description'];
			document.getElementsByName('resource-f3-'+i)[0].value=res[i-1]['location'];
			if (resource_media>0 && res[i-1].hasOwnProperty('media'))
				document.getElementsByName('resource-f4-'+i)[0].value=res[i-1]['media'];
		}
		listboite['resources']['compteur']=res.length;
	} else {
	// If errors
		message = 'ERROR: the uploaded file seems not a valid Maggot JSON file.';
		console.log(message)
		console.log(JSON.stringify(resp))
		alert(message);
		return false;
	}
	
};

// Reset form : Clean the textbox && Uncheck all checkboxes 
function reset_form()
{
	$('#formulaire').find('input[type=text]').each(function(){ $(this).val('') })
	Object.keys(checkboite).forEach( function(item, index) {
		chkbxdico[Object.keys(checkboite)[index]].forEach( function(term) {
			elem = item+term.replace(/ /g, "_");
			$("#"+elem).prop( "checked", false );
		})
	})
	Object.keys(multiboite).forEach( function(item, index) {
		var element = document.getElementById(item+'-sel')
		if (element != null) element.value = '';
	})
}


// Retrieve all field content
function toJSONString( form )
{
	var obj = {};
		obj[ 'profile' ] = url_schema;
	// Text
	var elements = form.querySelectorAll( "input.solo[type=text]" );
	for( var i = 0; i < elements.length; ++i ) {
		var element = elements[i];
		var name = element.name;
		var value = element.value;
		if (/^datatype-\d/.test(name) == true || /^valorisation-\d/.test(name) || /^resource-f\d/.test(name)) { continue; }
		// Checkbox datatype to add
		if( name && value.length>0) {
			obj[ name ] = value;
		}
	}
	// Textarea
	var elements = form.querySelectorAll( "textarea" );
	for( var i = 0; i < elements.length; ++i ) {
		var element = elements[i];
		var name = element.name;
		if (!name.includes('resource-f1-')){
			var value = element.value;
			if( name && value.length>0 && value != '-') obj[ name ] = value;
		}
	}
	// Select
	var elements = form.querySelectorAll( "select" );
	for( var i = 0; i < elements.length; ++i ) {
		var element = elements[i];
		var name = element.name;
		if (/^resource-f\d/.test(name)) { continue; }
		if (!name.includes('resource-f1-')) {
			var value = element.value;
			if( name && value.length>0 && value != '-') obj[ name ] = value;
		}
	}
	// Multi-select
	for (var key in multiboite) {
		var listTerms = [];
		if (multiboite.hasOwnProperty(key)) {
		var terms = $('#'+key+'-sel' ).val();
		if (terms.length>0) listTerms=terms.split( /,\s*/ );
		}
		if (listTerms.length) obj[key] = listTerms;
	}
	// CheckBoxes
	for (var key in chkbxdico) {
		if (chkbxdico.hasOwnProperty(key)) {
			var elements = form.querySelectorAll( "input."+key+":checked, input."+key+"[type=text]" );
			var listData = [];
			for( var i = 0; i < elements.length; ++i ) {
				var element = elements[i];
				var name = element.name;
				var value = element.value;
				if( value.length>0)
					listData.push(value);
			}
		}
		if (listData.length) obj[key] = listData;
	}
	// Resources
	R=formulaire.querySelectorAll( 'table.resource' )
	if (R.length>0) {
		var listData = [];
		for( var i = 0; i < R.length; ++i ) {
			k=0;
			res = {};
			// Type
			datatype=R[i].querySelector('select').value;
			if (datatype.length>0) res.datatype=datatype;
			// Media
			if (resource_media>0) { 
				res_media = R[i].querySelectorAll('input')[k].value;
				if (res_media.length>0) res.media=res_media;
				k++
			}
			// Description
			description = R[i].querySelectorAll('input')[k].value;
			if (description.length>0) res.description=description;
			k++
			// Location
			res_location = R[i].querySelectorAll('input')[k].value;
			if (res_location.length>0) res.location=res_location;
			if (res.hasOwnProperty('description') && res['description'].length>0)
				listData.push(res);
		}
		if (listData.length) obj['resources'] = listData;
	}
	
	return JSON.stringify( obj );
}

// Check if all required fields have an appropriate value
function check_required(json, required)
{
	var ret=true;
	obj=JSON.parse(json);
	keys = Object.keys(obj);
	var errmsg='';
	if (keys.includes('title')) {
		if (! obj['title'].match(/^[a-zA-Z0-9-_]+$/)) {
			errmsg=errmsg+"ERROR:\n"+nameofrequired['title']+" is limited to alphabetic characters, numbers, and underscore and dash characters\n\n"
			ret=false;
		}
		if (obj['title'].length>20) {
			errmsg=errmsg+"ERROR:\n"+nameofrequired['title']+" is limited to 20 characters\n\n"
			ret=false;
		}
		if (!ret) alert(errmsg);
	}
	if (ret) {
		errmsg += "=== WARNING ===\nThe following field(s) must be defined (required):\n";
		required.forEach(key => {
			if (!keys.includes(key) || obj[key].length==0) {
				errmsg=errmsg+"* "+nameofrequired[key]+"\n"
				ret=false;
			}
		})
		if (!ret) alert(errmsg+"\n ");
	}
	return ret;
}

function sendJSON(json, corename, message)
{
	indata = Base64.encode(json);
	resp='';
	$.ajaxSetup({async:false});
	$.get( "send", { name: corename, json: indata, body: encodeURI(message) } )
		.done(function(outdata) { resp=outdata; })
		.fail(function(outdata) { console.log(outdata); });
	$.ajaxSetup({async:true});
	if (DEBUG) console.log('SendJson:'+resp);
}

// Download as a JSON file
function saveFormAsTextFile(json, corename)
{
	var textToSaveAsBlob = new Blob([json], {type:"text/plain"});
	var textToSaveAsURL = window.URL.createObjectURL(textToSaveAsBlob);
	var fileNameToSaveAs = 'META_'+corename+'.json';
	var downloadLink = document.createElement("a");
	downloadLink.download = fileNameToSaveAs;
	downloadLink.innerHTML = "Download File";
	downloadLink.href = textToSaveAsURL;
	downloadLink.onclick = destroyClickedElement;
	downloadLink.style.display = "none";
	document.body.appendChild(downloadLink);
	downloadLink.click();
}

function destroyClickedElement(event)
{
	document.body.removeChild(event.target);
}

function getJsonName()
{
	var jsonName = Math.round(+new Date() / 1000);
	var labelFile = document.getElementById("inputGroupFile01");
	var fdata = document.forms["formulaire"];
	var fileName = labelFile.innerHTML;
	if (fileName.length>0) {
		pos1 = fileName.indexOf("META_")+5;
		pos2 = fileName.indexOf(".json");
		jsonName = fileName.substr(pos1, pos2-pos1);
	} else {
		datasetName = fdata.elements['title'].value;
		if (datasetName.length>0)
			jsonName = fdata.elements['title'].value;
	}
	return jsonName;
}

function generateJSON()
{
	var fdata = document.forms["formulaire"];
	var form2 = document.getElementById("saveForm");
	var jsonName = document.getElementById("jsonName");
	var jsonform = toJSONString( fdata );
	if (check_required(jsonform, required)) {
		jsonName.value = getJsonName();
		form2.style.display = 'block';
		$(form2).css('top', '300px')
		$(form2).css('left', ($(window).width()/2 - 250)+'px')
	}
}

function resetForm()
{
	reset_form();
	$('#dataset').val('').change();
	$('.resdiv').remove();
	listboite['resources']['compteur']=1;
}

(function()
{
	// Generate metadate : Generation of the file in json format
	document.addEventListener( "DOMContentLoaded", function()
	{
		var form2 = document.getElementById("saveForm");
		var fdata = document.forms["formulaire"];
		var jsonName = document.getElementById("jsonName");
		var jsonSend = document.getElementById("jsonSend");
		var jsonMsg = document.getElementById("jsonMsg");
		var btnGenerate = document.getElementById("btnGenerate");

		form2.addEventListener( "submit", function( e ) {
			e.preventDefault();
			var jsonform = toJSONString( fdata );
			if (check_required(jsonform, required)) {
				if (jsonSend.checked)
					sendJSON(jsonform, jsonName.value, $(jsonMsg).val());
				else
					saveFormAsTextFile(jsonform, jsonName.value);
			}
			this.style.display= "none";
		}, false);

		form2.addEventListener( "reset", function( e ) {
			e.preventDefault();
			this.style.display= "none";
		}, false);

		jsonSend.addEventListener( "change", function( e ) {
			if (this.checked) {
				$(form2).css('height','450px')
				$("#btnSave").prop("value", "Send")
				$(jsonMsg).show()
			} else {
				$(form2).css('height','250px')
				$("#btnSave").prop("value", "Save")
				$(jsonMsg).hide()
			}
		}, false);
	});
})();
