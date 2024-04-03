// global variables
var vartocomplete = [];     // List of information for each variable having an autocomplete entry (type, minlen, status, dictionary)
var externalDicoList = [];  // Cache for dictionary names already loaded from external dictionary (bioportal, skosmos) 
var internalDicoList = [];  // Cache for dictionary names already loaded from internal dictionary (cvlist)
var cvdir ='cvlist';        // Directory of the internal dictionaries

var Base64={_keyStr:"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=",encode:function(r){var e,t,o,a,h,d,C,c="",f=0;for(r=Base64._utf8_encode(r);f<r.length;)e=r.charCodeAt(f++),t=r.charCodeAt(f++),o=r.charCodeAt(f++),a=e>>2,h=(3&e)<<4|t>>4,d=(15&t)<<2|o>>6,C=63&o,isNaN(t)?d=C=64:isNaN(o)&&(C=64),c=c+this._keyStr.charAt(a)+this._keyStr.charAt(h)+this._keyStr.charAt(d)+this._keyStr.charAt(C);return c},decode:function(r){var e,t,o,a,h,d,C,c="",f=0;for(r=r.replace(/[^A-Za-z0-9\+\/\=]/g,"");f<r.length;)a=this._keyStr.indexOf(r.charAt(f++)),h=this._keyStr.indexOf(r.charAt(f++)),d=this._keyStr.indexOf(r.charAt(f++)),C=this._keyStr.indexOf(r.charAt(f++)),e=a<<2|h>>4,t=(15&h)<<4|d>>2,o=(3&d)<<6|C,c+=String.fromCharCode(e),64!=d&&(c+=String.fromCharCode(t)),64!=C&&(c+=String.fromCharCode(o));return Base64._utf8_decode(c)},_utf8_encode:function(r){r=r.replace(/\r\n/g,"\n");for(var e="",t=0;t<r.length;t++){var o=r.charCodeAt(t);o<128?e+=String.fromCharCode(o):o>127&&o<2048?(e+=String.fromCharCode(o>>6|192),e+=String.fromCharCode(63&o|128)):(e+=String.fromCharCode(o>>12|224),e+=String.fromCharCode(o>>6&63|128),e+=String.fromCharCode(63&o|128))}return e},_utf8_decode:function(r){for(var e="",t=0,o=c1=c2=0;t<r.length;)(o=r.charCodeAt(t))<128?(e+=String.fromCharCode(o),t++):o>191&&o<224?(e+=String.fromCharCode((31&o)<<6|63&(c2=r.charCodeAt(t+1))),t+=2):(e+=String.fromCharCode((15&o)<<12|(63&(c2=r.charCodeAt(t+1)))<<6|63&(c3=r.charCodeAt(t+2))),t+=3);return e}};

// Icon for mandatory field
var mandatory ='&nbsp;<i class="bi-asterisk" style="color:#db0d0d; font-size: 13px"></i>';

// Help icon linked to the corresponding metadata information
function htmlHelpIcon(item) {
	return '&nbsp;<a href="doc/meta#'+encodeURI(item)+'" target="doc"><i class="bi-question-octagon-fill" style="color:#1b82c1; font-size: 12px" onmouseover="$(this).css(\'font-size\',\'18px\');" onmouseleave="$(this).css(\'font-size\',\'12px\');"></i></a>'
}

// Download a File : 
//   * urlToSend : URL of the file to download
//   * filename : Name taken by the downloaded file
function downloadFile(urlToSend,filename='') {
	var req = new XMLHttpRequest();
	req.open("GET", urlToSend, true);
	req.responseType = "blob";
	req.onload = function (event) {
		var blob = req.response;
		var link=document.createElement('a');
		link.href=window.URL.createObjectURL(blob);
		if (filename.length>0)
			link.download=filename;
		else
			link.download=urlToSend.split(/[\\/]/).pop();
		link.click();
	};
	req.send();
}

// Test if a url exists, i.e. a javascript for example
function UrlExists(url)
{
	var http = new XMLHttpRequest();
	http.open('GET', 'check?file='+encodeURI(url), false);
	http.send(null);
	return http.responseText==1;
}

// Loads a javascript and triggers an action at the end of the load
function loadScript(url, active=0)
{
	$.ajax({
		url: url,
		async: false,
		dataType: "script",
		success: function(){ if (active) active_autocomplete();  }
	});
}

// Load either the last autocomplete script or the one specified by the index
function autocompleteScript(index=-1)
{
	var idx = (index<0) ? vartocomplete.length-1 : index;
	var active = (index<0) ? 0 : 1;
	if (idx>=0) {
		var dico=vartocomplete[idx]['dico'];
		do {
			if (externalDicoList.includes(dico) || internalDicoList.includes(dico)) break; // if dico already in cache
			url_script='js/autocomplete/'+dico+'.js' // external dictionary (bioportal, skosmos, ...) 
			if (UrlExists(url_script)) {
				if (DEBUG) console.log('autocompleteScript: Index='+idx+', External - loadScript '+url_script+', active='+active)
				loadScript(url_script,active);
				externalDicoList.push(dico); // put into cache
				if (! active) vartocomplete[idx]['listener'] = 1;
				break;
			}
			url_script=cvdir+'/'+dico+'/'+dico+'.js'; // internal dictionary
			if (UrlExists(url_script)) {
				internalDicoList.push(dico); // put into cache
				if (! active) break // script will be loaded later when clicking in the input box (cf active_update_autocomplete)
				if (DEBUG) console.log('autocompleteScript: Index='+idx+', Internal - loadScript '+url_script+', active='+active)
				loadScript(url_script,active);
				break;
			}
		} while (0);
	}
}

// Load a dictionary values
function get_dictionary_values(dico, merge=[])
{
	var tsvData='';
	var url_nocache = cvdir+'/'+dico+'/'+dico+'.txt?nocahe='+Math.random();
	
	$.ajaxSetup({async:false});
	$.get(url_nocache, function(data) { tsvData=data; });
	$.ajaxSetup({async:true});
	
	var lines=tsvData.split("\n");
	var result = [];
	for(var i=0;i<lines.length;i++) {
		var currentline=lines[i].split("\t");
		var thestr=''
		if (merge.length>0 && currentline[merge[0]].length>0)
		for(var j=0;j<merge.length;j++) {
			if (typeof merge[j] == 'number') thestr += currentline[merge[j]];
			if (typeof merge[j] == 'string') thestr += merge[j];
		}
		else if (currentline[0].length)
		thestr=currentline[0];
		if (thestr.length>0)
		eval(dico).push(thestr);
	}
}



/* Autocompletion */
function autocomplete(inp, arr, minlen=3)
{
	/*the autocomplete function takes two arguments,
	the text field element and an array of possible autocompleted values:*/
	var currentFocus;
	/*execute a function when someone writes in the text field:*/
	inp.addEventListener("input", function(e) {
		var a, b, i, val = this.value;
		/*close any already open lists of autocompleted values*/
		closeAllLists();
		if (!val || val.length < minlen) { return false;}
		currentFocus = -1;
		/*create a DIV element that will contain the items (values):*/
		a = document.createElement("DIV");
		a.setAttribute("id", this.id + "autocomplete-list");
		a.setAttribute("class", "autocomplete-items");
		/*append the DIV element as a child of the autocomplete container:*/
		this.parentNode.appendChild(a);
		/*for each item in the array...*/
		for (i = 0; i < arr.length; i++) {
			/*check if the item starts with the same letters as the text field value:*/
			pos = arr[i].toUpperCase().indexOf(val.toUpperCase());
			if (pos>-1) {
				/*create a DIV element for each matching element:*/
				b = document.createElement("DIV");
				/*make the matching letters bold:*/
				if (pos==0) {
					b.innerHTML = "<strong>" + arr[i].substr(0, val.length) + "</strong>";
					b.innerHTML += arr[i].substr(val.length);
				} else {
					b.innerHTML = arr[i].substr(0, pos);
					b.innerHTML += "<strong>" + arr[i].substr(pos, val.length) + "</strong>";
					b.innerHTML += arr[i].substr(pos+val.length);
				}
				/*insert a input field that will hold the current array item's value:*/
				b.innerHTML += "<input type='hidden' value='" + arr[i] + "'>";
				/*execute a function when someone clicks on the item value (DIV element):*/
				b.addEventListener("click", function(e) {
					/*insert the value for the autocomplete text field:*/
					inp.value = this.getElementsByTagName("input")[0].value;
					/*close the list of autocompleted values,
					(or any other open lists of autocompleted values:*/
					closeAllLists();
				});
				a.appendChild(b);
			}
		}
	});
	/*execute a function presses a key on the keyboard:*/
	inp.addEventListener("keydown", function(e) {
		var x = document.getElementById(this.id + "autocomplete-list");
		if (x) x = x.getElementsByTagName("div");
		if (e.keyCode == 40) {
			/*If the arrow DOWN key is pressed,
			increase the currentFocus variable:*/
			currentFocus++;
			/*and and make the current item more visible:*/
			addActive(x);
		} else if (e.keyCode == 38) { //up
			/*If the arrow UP key is pressed,
			decrease the currentFocus variable:*/
			currentFocus--;
			/*and and make the current item more visible:*/
			addActive(x);
		} else if (e.keyCode == 13) {
			/*If the ENTER key is pressed, prevent the form from being submitted,*/
			e.preventDefault();
			if (currentFocus > -1) {
				/*and simulate a click on the "active" item:*/
				if (x) x[currentFocus].click();
			}
		}
	});
	function addActive(x) {
		/*a function to classify an item as "active":*/
		if (!x) return false;
		/*start by removing the "active" class on all items:*/
		removeActive(x);
		if (currentFocus >= x.length) currentFocus = 0;
		if (currentFocus < 0) currentFocus = (x.length - 1);
		/*add class "autocomplete-active":*/
		x[currentFocus].classList.add("autocomplete-active");
	}
	function removeActive(x) {
		/*a function to remove the "active" class from all autocomplete items:*/
		for (var i = 0; i < x.length; i++) {
			x[i].classList.remove("autocomplete-active");
		}
	}
	function closeAllLists(elmnt) {
		/*close all autocomplete lists in the document,
		except the one passed as an argument:*/
		var x = document.getElementsByClassName("autocomplete-items");
		for (var i = 0; i < x.length; i++) {
			if (elmnt != x[i] && elmnt != inp) {
				x[i].parentNode.removeChild(x[i]);
			}
		}
	}
	/*execute a function when someone clicks in the document:*/
	document.addEventListener("click", function (e) {
		closeAllLists(e.target);
	});
}

//----------------------------------------
 /* Management of DIV flip */
//----------------------------------------

function switchTo(elementname, state)
{
	element = $_(elementname);
	if (element) {
		gfx = $_(elementname + 'img');
		if (state == 'on') {
			element.style.display = 'inline';
			gfx.src = 'img/arrow-down.png';
		}
		else {
			element.style.display = 'none';
			gfx.src = 'img/arrow-right.png';
		}
	}
}

function flipDisplay(elementname)
{
	element = $_(elementname);
	if (element)
		if (element.style.display == 'inline') switchTo(elementname, 'off');
		else                                   switchTo(elementname, 'on');
}

function flip_form(div)
{
	flipDisplay(div);
	divimg=div+'img';
	$_(divimg).onclick=function(){ flipDisplay(div); }
}

function flip_all(state)
{
	$('#formsearch').find('.flip').each(function(){
		if (state=='off') $(this).css('display','inline'); else $(this).css('display','none');
		flip_form($(this).attr('id'))
	})
}

//----------------------------------------
/* insert Formular elements */
//----------------------------------------

function cleanfield(elm) {
	result = theval = $(elm).val().trim();
	if (theval.substr(theval.length-1,1)==',') result = theval.substr(0,theval.length-1) ;
	$(elm).val(result)
	$('#'+$(elm).prop('id')+'-warn').css('display','none');
}

// Put the selected term from Bioportal results into the corresponding input box
function get_term_from_bpbox (bpinput) {
	var box = $(bpinput).attr('name');
	var selterm = $("input[name="+box+"]").val();
	var terms = $('#'+box+'-sel' ).val();
	if (terms.length>0) terms=terms.split( /,\s*/ ); else terms=[];
	terms.push( selterm ); 
	$('#'+box+'-sel').val(terms.join(', '));
	$("input[name="+box+"]").focus().val('');
}

// dock the results box to the input box
function dock_results_box() {
	do {
		var $focused = $(':focus');
		var className = $focused.attr('class');
		if (typeof className === "undefined") continue; // focused element must have a class
		if ($focused.scrollParent().css('overflow-y') !== 'scroll' ) continue; // focused element must have a scrollable parent
		if ($focused.scrollParent().attr('id') !== "fields") continue; // the scrollable parent must have the id 'fields'
		var bpok=false
		for(i=0; i<bioportal_sites.length; i++)
			if (className.match(new RegExp('^'+bioportal_sites[i].BP_matchString,'g'))) { bpok=true; break }
		if(!bpok) continue;  // the first class must correspond to a bioportal one
		var evt=0; $.each( $._data($focused.scrollParent().get(0),'events'), function(i,o) { if (i==='scroll') evt=1; })
		if (!evt) $focused.scrollParent().on('scroll', function(event){ dock_results_box(); }); // handle scroll event on the scrollable parent
		$result = $('.ac_results.'+$focused.attr('name'))
		if ( $result.css('display') !== 'block' ) continue; // the bioportal result box must be displayed
		$result.css('top', ($focused.offset().top+33)+'px')
	} while(false);
}

function insert_multiboite (identif_boite, req=false)
{
	var asterisk='';
	if (req) asterisk = mandatory
	label = multiboite[identif_boite]['titre'];

	disabled=''
	if (multiboite[identif_boite].hasOwnProperty('open') && ! multiboite[identif_boite]['open']) disabled=' disabled';
	var myfieldHTML ='<div class="form-group2 div-group ui-widget"><span class="labeldiv">'+label+'</span>'+asterisk+htmlHelpIcon(label)+'<br><input class="form-control form-control-sm" id="'+identif_boite+'-sel" name="'+identif_boite+'-sel" onfocus="$(\'#'+identif_boite+'-sel-warn\').css(\'display\',\'block\');" onfocusout="cleanfield(this);" onchange="statusentry=1;" '+disabled+'>';

	if (multiboite[identif_boite].hasOwnProperty('autocomplete')) {
		var nameList = multiboite[identif_boite]['autocomplete'];
		var msg = 'Warning: You should use the box below to search for terms by autocompletion. Anyway take care to separate each term with a comma followed by a space.';
		var bponto='', ac='', dico='';
		for(i=0; i<bioportal_sites.length; i++)  {
			if (nameList !== bioportal_sites[i].BP_NAME) continue
			if (DEBUG) console.log('multiselect: '+identif_boite+' autocomplete by '+bioportal_sites[i].BP_NAME+':'+multiboite[identif_boite]['onto'])
			onto='all'; if (multiboite[identif_boite].hasOwnProperty('onto')) { onto=multiboite[identif_boite]['onto']; onto=onto.split(':').join(','); }
			bponto=bioportal_sites[i].BP_matchString+'-'+onto+'-name ';
			break
		}
		if (bponto == '') {
			if (DEBUG) console.log('multiselect: '+identif_boite+' autocomplete by dico:'+nameList)
			minlen=3; if (multiboite[identif_boite].hasOwnProperty('min')) minlen=multiboite[identif_boite]['min'];
			vartocomplete[vartocomplete.length]= { variable : identif_boite, dico: nameList, minlen: minlen, listener: 0, type: 1 };
			autocompleteScript()
			ac=' autocomplete ';
			dico=' dico="'+nameList+'"';
		}
		myfieldHTML += '<div id="'+identif_boite+'-sel-warn" class="warnmessage"><font size="-1"><i>'+msg+'</i></font></div><p class="selval"><span class="selval">Search a value</span>:&nbsp;<input class="'+bponto+'input-selval'+ac+'"'+dico+'id="'+identif_boite+'" name="'+identif_boite+'" placeholder="enter the first letters">'
		if (disabled.length>0)
			myfieldHTML += '<button type="button" class="btn btn-primary btn-xs" onclick="$(\'#'+identif_boite+'-sel\').val(\'\');">Reset</button>';
		myfieldHTML += '</p></div>';
	}
	return myfieldHTML;
}

function insert_checkboite (identif_boite, additem=true, req=false)
{
	var divclass=asterisk=plh=ac=dico='';
	if (req) asterisk = mandatory
	var myfieldHTML =
		'<div class="form-group2 div-group box-style-1">\
			<div style="padding:3px; background-color:#fff;">\
				<div id="'+identif_boite+'" class="form-group '+identif_boite+'">\
					<span class="labeldiv">'+checkboite[identif_boite]['titre']+'</span>'+asterisk+htmlHelpIcon(checkboite[identif_boite]['titre'])+'\
					<fieldset></fieldset>\
				</div>';
	
	if (additem && checkboite[identif_boite]['open']) {
		if (checkboite[identif_boite].hasOwnProperty('autocomplete')) {
			var nameList = checkboite[identif_boite]['autocomplete'];
			minlen=3; if (checkboite[identif_boite].hasOwnProperty('min')) minlen=checkboite[identif_boite]['min'];
			vartocomplete[vartocomplete.length]= { variable : identif_boite+'-1', dico: nameList, listener: 0, type: 0, minlen: minlen };
			autocompleteScript()
			divclass='class="autocomplete"';
			plh='placeholder="enter the first three letters"';
			ac=' autocomplete ';
			dico=' dico="'+nameList+'"';
		}
		checkboite[identif_boite]['compteur'] =  2;
		myfieldHTML += '\
				<br><span class="labeldiv">OR enter an item (if missing in the above options) :</span>\
				<div class="field_wrapper '+identif_boite+'">\
					<div '+divclass+' style="width:100%">\
						<input class="form-control form-control-sm '+identif_boite+ac+'"'+dico+'" type="text" name="'+identif_boite+'-1" id="'+identif_boite+'-1" '+plh+'value=""/>\
						<a href="javascript:void(0);" class="add_button '+identif_boite+'" title="Add field"><button type="button" class="btn btn-primary btn-xs" id="add_button_'+identif_boite+'"><i class="fa fa-plus-circle" aria-hidden="true"></i> Add a field</button></a>\
						<input type="hidden" name="compteur" value="2" id="compteur"/>\
					</div>\
				</div>\
			</div>\
		</div>';
	} else {
		myfieldHTML += '</div>\</div>';
	}
	return myfieldHTML;
}

function insert_listboite (identif_boite, req=false, shrunk=false)
{
	var mywidth='style="width: 100%"';
	if (! shrunk && listboite[identif_boite].hasOwnProperty('width'))
		mywidth = 'style="width:'+listboite[identif_boite]['width']+'"';
	var divclass='class="form-group2 '+identif_boite+'" '+mywidth;
	var asterisk=''; if (req) asterisk = mandatory
	var ac='', dico='';
	if (listboite[identif_boite].hasOwnProperty('terms')) {
		var nameList = listboite[identif_boite]['terms']
		if (DEBUG) console.log('dropbox: '+identif_boite+' autocomplete by dico:'+nameList)
		vartocomplete[vartocomplete.length]= { variable : identif_boite, dico: nameList, listener: 0, type: 2};
		autocompleteScript()
		ac=' autocomplete ';
		dico=' dico="'+nameList+'"';
	}
	var myfieldHTML =
		'<div '+divclass+'id="'+identif_boite+'">\
			<span class="labeldiv">'+listboite[identif_boite]['titre']+'</span>'+asterisk+htmlHelpIcon(listboite[identif_boite]['titre'])+'\
			<fieldset>\
				<select id="'+identif_boite+'_select" name="'+identif_boite+'" class="form-control form-control-sm '+identif_boite+ac+'"'+dico+'" onchange="statusentry=1;"></select>\
			</fieldset>\
		</div>';
	return myfieldHTML;
}

function insert_txtboite (identif_boite, req=false, shrunk=false)
{
	var divclass=asterisk=mywidth=ac=plh=dico='';
	mywidth = 'style="width: 100%"';
	if (! shrunk && txtboite[identif_boite].hasOwnProperty('width'))
		mywidth = 'style="width:'+txtboite[identif_boite]['width']+'"';
	if (txtboite[identif_boite].hasOwnProperty('autocomplete')) {
		var nameList = txtboite[identif_boite]['autocomplete']
		minlen=3; if (txtboite[identif_boite].hasOwnProperty('min')) minlen=txtboite[identif_boite]['min'];
		if (DEBUG) console.log('Textbox: '+identif_boite+' autocomplete by dico:'+nameList)
		vartocomplete[vartocomplete.length]= { variable : identif_boite, dico: nameList, listener: 0, type: 0, minlen: minlen };
		autocompleteScript()
		ac='autocomplete ';
		plh='placeholder="enter the first three letters"';
		dico=' dico="'+nameList+'"';
	}
	divclass='class="form-group2 '+ac+'" '+mywidth;
	if (req) asterisk = mandatory
	var myfieldHTML =
		'<div '+divclass+'>\
			<span class="labeldiv">'+ txtboite[identif_boite]['titre'] +'</span>'+asterisk+htmlHelpIcon(txtboite[identif_boite]['titre'])+'\
			<input class="form-control form-control-sm solo '+ac+'"'+dico+' type="text" name="'+ identif_boite +'" id="'+ identif_boite +'" '+ plh  +mywidth +'" onchange="statusentry=1;"/>\
		</div>';
	return myfieldHTML;
}

function insert_dateboite (identif_boite, req=false, shrunk=false)
{
	var divclass=asterisk=mywidth=ac=plh=dico='';
	mywidth = 'style="width: 100%"';
	if (! shrunk && dateboite[identif_boite].hasOwnProperty('width'))
		mywidth = 'style="width:'+dateboite[identif_boite]['width']+'"';
	divclass='class="form-group2 '+ac+'" '+mywidth;
	if (req) asterisk = mandatory
	plh='placeholder="yyyy-mm-dd"';
	var myfieldHTML =
		'<div '+divclass+'>\
			<span class="labeldiv">'+ dateboite[identif_boite]['titre'] +'</span>'+asterisk+htmlHelpIcon(dateboite[identif_boite]['titre'])+'\
			<input class="form-control form-control-sm solo '+ac+'"'+dico+' type="text" name="'+ identif_boite +'" id="'+ identif_boite +'" '+ plh  +mywidth +'" onchange="statusentry=1;"/>\
		</div>';
	return myfieldHTML;
}

function insert_areaboite (identif_boite, req=false)
{
	var asterisk=''; if (req) asterisk = mandatory
	var myfieldHTML =
		'<div class="form-group2 div-group">\
			<span class="labeldiv">'+ areaboite[identif_boite]['titre'] +'</span>'+asterisk+htmlHelpIcon(areaboite[identif_boite]['titre'])+'\
			<textarea class="form-control form-control-sm" name="'+ identif_boite +'" rows="'+ areaboite[identif_boite]['rows'] +'" cols="'+ areaboite[identif_boite]['cols'] +'" id="'+ identif_boite +'" onchange="statusentry=1;"></textarea>\
		</div>';
	return myfieldHTML;
}

function add_bouton (chk_box_name)
{
	var cur_cpt = checkboite[chk_box_name]['compteur'];
	var targetDOM = '.field_wrapper.' + chk_box_name;
	var ac=dico=''
	if(cur_cpt < maxField){
		cur_cpt++; //Increment counter
		checkboite[chk_box_name]['compteur'] =  cur_cpt;
		var divclass=plh=ac=dico='';
		if (checkboite[chk_box_name].hasOwnProperty('autocomplete')) {
			var nameList = checkboite[chk_box_name]['autocomplete']
			vartocomplete[vartocomplete.length]= { variable : chk_box_name+'-'+ cur_cpt, dico: nameList, listener: 0, type: 0 };
			divclass='class="autocomplete"';
			plh='placeholder="enter the first three letters"';
			ac=' autocomplete';
			dico=' dico="'+nameList+'"';
		}
		var fieldHTML = '<div '+divclass+' style="width:100%"><br/><input type="text" class="form-control form-control-sm '+ chk_box_name + ac + '"' + dico + ' name="'+ chk_box_name +'-'+ cur_cpt +'" id="'+ chk_box_name +'-'+ cur_cpt +'" '+ plh +' value=""/><a href="javascript:void(0);" class="remove_button '+ chk_box_name +'"><button type="button" class="btn btn-danger btn-xs" id="addrow">Remove the line</button></a></div>';
		$(targetDOM).append(fieldHTML); //Add html
		active_autocomplete()
	}
}

// Resources
function insert_resource(id, resTypes)
{
	opthml_1='<option value="None">---</option>';
	for (key in resTypes) opthml_1=opthml_1+'<option value="'+resTypes[key]+'">'+resTypes[key]+'</option>';
	var myfieldHTML = '<table class="resource" width="100%"><tr><td width="200px"><span class="labeldiv">Resource Type</span></td><td><select id="resource-f1-'+id+'" name="resource-f1-'+id+'" class="form-control form-control-sm resource" onchange="statusentry=1;">'+opthml_1+'</select></td></tr>';

	if (resource_media>0) {
		vartocomplete[vartocomplete.length]= { variable : 'resource-f4-'+id, dico: 'mediatypes', minlen: 3, listener: 1, type: 0 };
		autocompleteScript()
		myfieldHTML += '<tr><td><span class="labeldiv">Media Type</span></td><td><div class="autocomplete form-group" style="width: 100%"><input class="form-control form-control-sm solo autocomplete" dico="mediatypes" type="text" name="resource-f4-'+id+'" id="resource-f4-'+id+'" placeholder="e.g. pdf, csv, xml, png, json, tab-separated-values, openxml, opendoc, msexcel, msword, ..." style="width: 100%" onchange="statusentry=1;"/></div><td></tr>';
	}

	myfieldHTML += '<tr><td><span class="labeldiv">Description</span></td><td><input class="form-control form-control-sm resource" type="text" name="resource-f2-'+id+'" value="" placeholder="enter a very short description" onchange="statusentry=1;"><td></tr><tr><td><span class="labeldiv">Location</span></td><td><input class="form-control form-control-sm resource" type="text" name="resource-f3-'+id+'" value="" placeholder="enter either an URL resource or a full path on a backup disk" onchange="statusentry=1;"><td></tr></table>';
	return myfieldHTML;
}

function insert_resources(id)
{
	var myfieldHTML =
	'<div class="box-style-1"><div style="padding:3px; background-color:#fff;"><div class="resource_wrapper"><div>'+insert_resource(id, listdico['resources'])+'<div><a href="javascript:void(0);" class="add_button_r resource" title="Add resource"><button type="button" class="btn btn-primary btn-xs" id="add_button_r"><i class="fa fa-plus-circle" aria-hidden="true"></i> Add a resource</button></a>'+htmlHelpIcon('RESOURCES')+'</div></div></div></div></div><br/>';
	return myfieldHTML;
}

function add_bouton_r()
{
	var cur_cpt = listboite['resources']['compteur'];
	if(cur_cpt < maxResource){
		cur_cpt++; //Increment counter
		listboite['resources']['compteur'] =  cur_cpt;
		var fieldHTML = '<div><br/>'+insert_resource(cur_cpt, listdico['resources'])+'<a href="javascript:void(0);" class="remove_button_r"><button type="button" class="btn btn-danger btn-xs" id="addrow"><i class="fa fa-plus-circle" aria-hidden="true"></i> Remove the resource</button></a></div>';
		$('.resource_wrapper').append(fieldHTML); //Add html
		if (resource_media>0)
			active_autocomplete();
	}
}

//----------------------------------------
// Fill checkboxes && dropboxes with predefined vocabulary
//----------------------------------------

function fill_elements (search=false)
{
	//==== checkbox options ====
	for (var key in chkbxdico) {
		if (chkbxdico.hasOwnProperty(key)) {
			var obj = chkbxdico[key];
			var localcpt = maxField;
			for( var i = 0; i < obj.length; ++i ) {
				localcpt++;
				idobj=key+obj[i].replace(/ /g, "_");
				var htmltemplate = '<div class="form-check form-check-inline">\
									<span class="form-check-label">\
									<input type="checkbox" name="'+ key + '-' + localcpt + '" value="'+ obj[i] +'" class="form-check-input '+ key +'" id="'+ idobj +'" onchange="statusentry=1;">\
									'+ obj[i] +'</span></div>';
				$('#' + key + ' fieldset').append(htmltemplate);
			}
		}
	}
	
	//==== dropbox options ====
	for (var key in listdico) {
		if (search) {
			var htmltemplate = '<option value="All">All</option>';
			$('#'+key+' fieldset select').append(htmltemplate);
		}
		if (listdico.hasOwnProperty(key)) {
		var obj = listdico[key];
		for( var i = 0; i < obj.length; ++i ) {
				var htmltemplate = '<option value="'+ obj[i] +'">'+ obj[i] +'</option>';
				$('#'+key+' fieldset select').append(htmltemplate);
		}
		}
	}
}

//----------------------------------------
// Activate the all Autocomplete if defined
//----------------------------------------

function active_autocomplete(set_all=0)
{
	do {
		if (vartocomplete.length==0) break;
		for( var i = 0; i < vartocomplete.length; ++i ) {
			if (set_all==0 && vartocomplete[i]['listener']==0) continue;
			if (vartocomplete[i]['dico'].length>15 || vartocomplete[i]['dico'].search("[,/?()=:;@$.]")>=0) continue;
	
			var listTerms=[]
			try {
				listTerms = eval(vartocomplete[i]['dico']);
				vartocomplete[i]['listener']=0;
			} catch (e) {
				console.log('active_autocomplete - eval:'+e.message)
				vartocomplete[i]['listener']=1;
				break
			}
	
			// Type 0 : checkboite, txtboite, resource (media)
			if (vartocomplete[i]['type']==0) {
				try {
					var key=vartocomplete[i]['variable']
					if (DEBUG) console.log('active_autocomplete 0: '+key+', dico: '+vartocomplete[i]['dico'])
					autocomplete(document.getElementById(key), listTerms, vartocomplete[i]['minlen']);
				} catch(e) {}
				continue;
			}
	
			// Type 1 : multiboite (dico)
			if (vartocomplete[i]['type']==1) {
			box = vartocomplete[i]['variable'];
				if (DEBUG) console.log('active_autocomplete 1: '+box+', dico: '+vartocomplete[i]['dico'])
				$('#'+box).keydown(function(event){
					if(event.keyCode == 13) {
						event.preventDefault();
						box=$(event.target).prop('id')
						var terms = $('#'+box+'-sel' ).val();
						if (terms.length>0) terms=terms.split( /,\s*/ ); else terms=[];
						terms.push( this.value ); $('#'+box+'-sel').val(terms.join(', '));
						this.value='';
						$('#'+box+'-sel').focus().val($('#'+box+'-sel').val());
						return false;
					}
				});
				$( "#"+box ).autocomplete({
					autoFocus: true,
					source: listTerms,
					minLength: vartocomplete[i]['minlen'],
					delay: 500,
					select: function( event, ui ) {
						box=$(event.target).prop('id')
						var terms = $('#'+box+'-sel' ).val();
						if (terms.length>0) terms=terms.split( /,\s*/ ); else terms=[];
						terms.push( ui.item.value ); $('#'+box+'-sel').val(terms.join(', '));
						$(this).focus().val('');
						return false;
					}
				//focus: function() { return false; }
				})
				continue;
			}
	
			// Type 2 : listboite
			if (vartocomplete[i]['type']==2) {
				var key=vartocomplete[i]['variable']
				if (DEBUG) console.log('active_autocomplete 2: '+key+', dico: '+vartocomplete[i]['dico'])
				var htmltemplate = '';
				for( var j = 0; j < listTerms.length; ++j )
					htmltemplate += '<option value="'+ listTerms[j] +'">'+ listTerms[j] +'</option>';
					$('#'+key+'_select').append(htmltemplate);
				continue;
			}
		}
	} while (false);
}

function active_update_autocomplete()
{
	$('#fields').click(function(event) {
		do {
			var $focused = $(':focus');
			var className = $focused.attr('class');
			if (typeof className === "undefined") break; // focused element must have a class ...
			if ( ! $focused.hasClass('autocomplete') ) break; // ... named 'autocomplete'
			var dicoName = $focused.attr('dico');
			if (typeof dicoName === "undefined") break; // focused element must have a dico name
			if (externalDicoList.includes(dicoName)) break; // if external dico already in cache 
			$focused.addClass('ac_loading');
			var idName = $focused.attr('id');
			var index=-1;
			for( var i = 0; i < vartocomplete.length; ++i ) {
				if (vartocomplete[i]['dico']==dicoName && vartocomplete[i]['variable']==idName) {
					vartocomplete[i]['listener']=1;
					index=i;
					break;
				}
			}
			if (index<0) break
			internalDicoList = jQuery.grep(internalDicoList, function(value) { // remove internal dico from cache 
					return value !== dicoName;
			});
			if (DEBUG) console.log('active_update_autocomplete : '+idName+', dico: '+dicoName)
			autocompleteScript(index)
			if($.active > 0)
				$(document).ajaxStop(function(){ 
					$(document).unbind("ajaxStop"); window.setTimeout( $(':focus').removeClass('ac_loading'), 300);
				});
			else
				$(':focus').removeClass('ac_loading')
		} while(false);
	});
}
