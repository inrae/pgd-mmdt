<!DOCTYPE html>
<!-
  Copyright: (c) INRAE - 2022-2023
->
<html lang="en" >
<head>
	<title>Maggot - Metadata entry</title>
	<?php echo $HEADMAIN ?>

	<style>::-webkit-scrollbar{ width: 5px; }</style>
	<script src="js/entry.js"></script>
	<script src="js/autocomplete/resourcetypes.js"></script>
	<script type="text/javascript">
	var DEBUG = "<? echo $_GET['debug'] ?>";
	var SEND = "<? echo ! is_null($sendMail) ?>";
	var AJAXWAIT = "<? echo isset($_GET['wait']) ? $_GET['wait'] : $AJAXWAIT ?>";
	var arrItems;
	var url_schema = '<?php echo $url_schema; ?>';
	var resource_media = '<?php echo $RESMEDIA; ?>';
	var maxField = 10; //Checkboxes increment limitation
	var maxResource = 20; //Resources increment limitation
	var nameofrequired = [];
	var extern= <?php $EXTERN = isset($_GET['extern']) ? 1 : $EXTERN; if($EXTERN){ echo "true"; } else { echo "false"; } ?> ;
	var cvdir=<?php echo "'".$cvdir."'" ?>;
	var horiztabs = 1;
	var statusentry=0;
	var maxWidth=1200;
	if ( $( window ).width()>maxWidth ) horiztabs = 0;

	function $_(id) { return document.getElementById(id) }

	$(document).ready(function() {

		// Load 'json/options.json' into the arrItems array
		$.ajaxSetup({async:false});
			$.getJSON("conf/options.json", function (data) {
				arrItems = [];
				$.each(data, function (index, value) {
					arrItems.push(value);
				});
			});
		$.ajaxSetup({async:true});

		// Get the different dictionnaries
		sections = arrItems[0];    // sections which gathered one or severals variable
		required = arrItems[2];    // required fields
		checkboite = arrItems[4];  // list of the checkboxes along with their parameters
		listboite = arrItems[5];   // list of the dropboxes along with their parameters
		multiboite = arrItems[6];  // list of multi-select input along with their parameters
		txtboite = arrItems[7];    // list of the input boxes along with their parameters
		areaboite = arrItems[8];   // list of the textarea boxes along with their parameters
		dateboite = arrItems[9];   // list of the date boxes along with their parameters
		chkbxdico = arrItems[10];  // dictionnaries for checkboxes
		listdico = arrItems[11];   // dictionnaries for dropboxes

		var cnt=0;
		var myhtml='<div id="tabs"><ul>';
		for (var key in sections['labels']) {
			cnt++;
			frequired=false;
			for(var key2 in sections[key])
			if(required.includes(sections[key][key2])) { frequired=true; break; }
			asterisk='';
			if (frequired) asterisk=' <i class="bi-asterisk" style="color:#db0d0d; font-size: 13px"></i>';
			myhtml += '<li><b><a href="#tabs-'+cnt+'">'+key.toUpperCase()+asterisk+'</a></b></li>'
		}
		myhtml += '</ul>';

		cnt=0;
		// Insert HTML fields defined in arrItems
		for (var key in sections['labels']) {
			if (key=='resources') continue;
			cnt++;
			myhtml+='<div class="tab-content" id="tabs-'+cnt+'">'
			var cnt2=0, br=1;
			// Sections
			for(var key2 in sections[key]) {
				var fwidth=false; cnt2++;
				var mykey = sections[key][key2];
				var frequired = required.includes(mykey)
				var subhtml='';
			// txtboite
				if (txtboite.hasOwnProperty(mykey)) {
					fwidth=txtboite[mykey].hasOwnProperty('width');
					if (frequired) { nameofrequired[mykey]=txtboite[mykey]['titre']; }
					subhtml=insert_txtboite(mykey, req=frequired);
			// dateboite
				} else if (dateboite.hasOwnProperty(mykey)) {
					fwidth=dateboite[mykey].hasOwnProperty('width');
					if (frequired) { nameofrequired[mykey]=dateboite[mykey]['titre']; }
					subhtml=insert_dateboite(mykey, req=frequired);
			// listboite
				} else if (listboite.hasOwnProperty(mykey)) {
					fwidth=listboite[mykey].hasOwnProperty('width');
					if (frequired) { nameofrequired[mykey]=listboite[mykey]['titre']; }
					subhtml=insert_listboite(mykey, req=frequired);
			// checkboite
				} else if (checkboite.hasOwnProperty(mykey)) {
					if (frequired) { nameofrequired[mykey]=checkboite[mykey]['titre']; }
					subhtml=insert_checkboite(mykey, additem=!extern, req=frequired);
					checkboite[mykey]['compteur'] = 1;
			// multiboite
				} else if (multiboite.hasOwnProperty(mykey)) {
					if (frequired) { nameofrequired[mykey]=multiboite[mykey]['titre']; }
					subhtml=insert_multiboite (mykey, req=frequired);
			// areaboite
				} else if (areaboite.hasOwnProperty(mykey)) {
					if (frequired) { nameofrequired[mykey]=areaboite[mykey]['titre']; }
					subhtml=insert_areaboite(mykey, req=frequired);
				}
			// Output html
				if (cnt2>1 && (!fwidth || (fwidth && br==1))) myhtml+= '<br><br>';
				if (!fwidth) br=1; else if( fwidth && br==1) br=0;
				myhtml+=subhtml;
				if (fwidth) myhtml+='&nbsp;&nbsp;&nbsp;';
			}
			myhtml+='</div>';
		}

		// Get "Resources" types
		if (!listdico.hasOwnProperty('resources') || listdico['resources'].length==0) {
			//resourcetypes=[];
			//get_terms_from_bioportal('DATACITE-VOCAB', 'http://purl.org/datacite/v4.4/ResourceTypeGeneral', 'resourcetypes');
			listdico['resources'] = resourcetypes;
		}

		// "Resources" section
		if (sections['labels'].hasOwnProperty('resources')) {
			id=1;
			listboite['resources'] = { 'compteur': id };
			cnt++;
			myhtml+='<div class="tab-content" id="tabs-'+cnt+'">'
			myhtml+=insert_resources(id)
			myhtml+='</div>';
		}

		myhtml += '</div>';
		$('#fields').append(myhtml);

		// Add Listeners for dateboxes
		for (var key in dateboite) {
			$( "#"+key ).datepicker();
			$( "#"+key ).datepicker("option", "dateFormat", "yy-mm-dd"); //
		}

		// Add Listeners for checkoxes
		for (var key in checkboite)
			if (checkboite[key].hasOwnProperty('open') && checkboite[key]['open']==true)
				add_button_click(key);

		function add_button_click(key) {
			$('.add_button.'+key).click( function(e) { add_bouton(key); })
			$('.field_wrapper.'+key).on('click', '.remove_button.'+key, function(e){
				e.preventDefault();
				content = $(this).parent('div').find( "input" ).val();
				if (content.length==0 || confirm("Suppression confirmation:\nEither OK or Cancel.")) {
					$(this).parent('div').remove(); //Remove field html
					checkboite[key]['compteur']--; //Decrement field counter
				}
			});
		}

		// Add Listeners for resources
		if (sections['labels'].hasOwnProperty('resources')) {
			$('.add_button_r').click( function(e) { add_bouton_r(); })
			$('.resource_wrapper').on('click', '.remove_button_r', function(e){
				e.preventDefault();
				arrobj = $(this).parent('div').find( "input" );
				content=false;
				for(var i=0; i<arrobj.length; i++)
					if ($( arrobj[i] ).val().length>0) content=true;
				if (! content || confirm("Suppression confirmation:\nEither OK or Cancel.")) {
					$(this).parent('div').remove(); //Remove resource html
					listboite['resources']['compteur']--; //Decrement resource counter
				}
			});
		}

		// Fill checkboxes && dropboxes with predefined vocabulary
		fill_elements ();

		// Enable AutoComplete Update
		active_update_autocomplete();

		// Handling the 'autocomplete_selected' event of the 'Bioportal' widget
		// so that the selected term can be put into the corresponding input box
		$(document).on( "autocomplete_selected", function(event, params) {
			get_term_from_bpbox(params.input);
		});

		// Handling the 'show_results' event of the 'Bioportal' widget
		// so that the results box is docked to the corresponding input box when displayed
		$(document).on('show_results', function(event){ dock_results_box(); });

		if (horiztabs) {
			$( "#tabs" ).tabs().removeClass( "ui-corner-left" );
		} else {
			$( "#tabs" ).tabs().addClass( "ui-tabs-vertical ui-helper-clearfix" );
			$( "#tabs li" ).removeClass( "ui-corner-top" ).addClass( "ui-corner-left" );
		}

	});
    </script>
</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_main ?>

<div id='loadmessage' class="loadmessage"><i>Loading external vocabulary</i>&nbsp;&nbsp;&nbsp;<img src="img/dots-loading.gif" width="30px"></div>
<br>

<form enctype="multipart/form-data" autocomplete="off" action="#" method="post">
	<table style="border:0; padding:0; max-width:1200px; width:100%">
	<tr><td style="width:10px"><img src="img/arrow-right.png" width="20px" onclick="flip_form('SECTION1')" id="SECTION1img" class="link"> </td><td><span class="sectiontitle">Pre-fill the form</span></td></tr>
	<tr><td></td><td><div class="flip" id="SECTION1" style="display: none;">
		<div class="input-group mb-3">
			<div class="input-group-prepend">
				<span class="input-group-text">Select a JSON file</span>
			</div>
			<div class="custom-file">
				<input type="file" class="custom-file-input" id="selectFiles">
				<label class="custom-file-label" id="inputGroupFile01" for="selectFiles"></label>
			</div>
		</div>
	</div></td></tr>
	</table>
	<br>
</form>

<!-- Save as JSON -->
<div id="saveForm" class="saveForm" enctype="multipart/form-data" autocomplete="off" action="#" style="display: none;">
	<strong>Generate a JSON file</strong>
	<form id ="saveform" name="saveform" >
		<br><span>Filename:</span>&nbsp;&nbsp;<b>META_</b>&nbsp;<input id="jsonName" name="jsonName" type="text">&nbsp;<b>.json</b><br><br>
		<div id="divSend">
			<input type="checkbox" id="jsonSend" name="jsonSend" value="">
			<label id="labelSend" for="SEND">Send the metadata file to the data manager(s)</label><br><br>
			<textarea class="form-control form-control-sm" id="jsonMsg"  name="jsonMsg" rows="8" style="display: none;" placeholder="Please put your comment here"></textarea>
		</div><br>
		<span><center>
			<input type="submit" id="btnSave" value="Save" class="btn btn-primary" />
			<input type="reset" value="Cancel" class="btn btn-primary"/>
		</center></span>
	</form>
</div><!-- /div Save as JSON -->

<form id="submitForm" enctype="multipart/form-data" autocomplete="off" action="#" method="post">
	<center><p>
		<input type="submit" value="Generate the metadata file" class="btn btn-primary" style="width: 250px;" />
		<input type="reset" value="Empty the form" class="btn btn-primary" style="width: 250px;" onclick="window.location.reload();"/>
	</p></center>
</form>


<form id="formulaire" enctype="multipart/form-data" autocomplete="off" action="#" method="post">
	<div id='fields'></div>
	<p><span style="color:#db0d0d;"><i class="bi-asterisk"></i></span>&nbsp;<i>mandatory fields</i></p>
</form>

<br/>

<script>
	// Show file name select for loading : https://www.codeply.com/p/LtpNZllird
	document.querySelector('.custom-file-input').addEventListener('change',function(e){
	var fileName = document.getElementById("selectFiles").files[0].name;
	var nextSibling = e.target.nextElementSibling
	nextSibling.innerText = fileName
	var file = document.getElementById("selectFiles").files;
	reset_form();
	load_data(file);
	});
	$("[name='btn-input']").removeClass('btn-primary');  $("[name='btn-input']").addClass('menu-select');

	// Disable the Send button if sendMail id not configurated
	if (SEND.length==0) {
		$("#jsonSend").attr("disabled", true);
		$("#divSend").hide();
		if (DEBUG) console.log('SEND is disable');
	}

	function final_loading() {
		if (DEBUG) console.log('--- BEGIN final_loading ...')
		bioportal_setup();
		active_autocomplete()
		$('#loadmessage').css('display','none');
		if (DEBUG) console.log('--- END final_loading ...')
		if (DEBUG) for(i=0; i<vartocomplete.length; i++) console.log(vartocomplete[i])
	}

	function differ_ajaxtest() {
		if($.active > 0) {
			if (DEBUG) console.log('Wait for AJAX to finish ...')
			$(document).ajaxStop(function(){ $(document).unbind("ajaxStop"); setTimeout(final_loading,300); });
		}
		else {
			if (DEBUG) console.log('Wait '+AJAXWAIT+' ms ...')
			setTimeout(final_loading,AJAXWAIT);
		}
	}

	$(window).on('load', function(){
		setTimeout(differ_ajaxtest, 1000)
	})

	$(window).on('resize', function(){
		if ( $( window ).width()>maxWidth ) {
		$( "#tabs" ).tabs().addClass( "ui-tabs-vertical ui-helper-clearfix" );
		$( "#tabs li" ).removeClass( "ui-corner-top" ).addClass( "ui-corner-left" );
		} else {
		$( "#tabs" ).tabs().removeClass( "ui-tabs-vertical ui-helper-clearfix" );
		$( "#tabs li" ).removeClass( "ui-corner-left" ).addClass( "ui-corner-top" );
		}
	})

	window.addEventListener('beforeunload',(event) =>{
		if (statusentry) event.returnValue = `Are you sure you want to leave?`;
	});
</script>
