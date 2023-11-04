<!DOCTYPE html>
<!-
  Copyright: (c) INRAE - 2022-2023
->
<html lang="en" >
<head>
	<title>Maggot - Search</title>
	<?php echo $HEADMAIN ?>

	<style>::-webkit-scrollbar{ width: 0px; }</style>
	<script src="js/search.js"></script>
	<script type="text/javascript">
	var arrItems;
	var maxField = 10; //Checkboxes increment limitation
	var maxWidth=1200;
	var cvdir=<?php echo "'".$cvdir."'" ?>;
	var signsort = Array();

	function $_(id) { return document.getElementById(id) }

	$(document).ready(function() {
		// Load json from user
		$('.load_data').click(function() {
			var file = document.getElementById("selectFiles").files;
			load_data(file);
		});

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
		sections = arrItems[1];    // sections which gathered one or severals variable
		shortview = arrItems[3];   // sections which gathered one or severals variable
		checkboite = arrItems[4];  // list of the checkboxes along with their parameters
		listboite = arrItems[5];   // list of the dropboxes along with their parameters
		multiboite = arrItems[6];  // list of multi-select input along with their parameters
		txtboite = arrItems[7];    // list of the input boxes along with their parameters
		areaboite = arrItems[8];   // list of the textarea boxes along with their parameters
		dateboite = arrItems[9];   // list of the date boxes along with their parameters
		chkbxdico = arrItems[10];  // dictionnaries for checkboxes
		listdico = arrItems[11];   // dictionnaries for dropboxes

		// Insert HTML fields defined in arrItems
		cnt=0;
		myhtml='<table style="border:0; padding:0; max-width:1200px; width:100%; background-color: #ffffff;">'
		shrunk = $( window ).width()>maxWidth;
		for (var key in sections['labels']) {
			cnt++;
			if (key=='resources') continue;
			myhtml=myhtml+'<tr><td style="width:10px"><img src="img/arrow-right.png" width="20px" onclick="flip_form(\'SECTION'+cnt+'\')" id="SECTION'+cnt+'img" class="link"></td><td><span class="sectiontitle">'+sections['labels'][key]+'</span></td></tr><tr><td></td><td><DIV class="flip" id="SECTION'+cnt+'" style="display: none;">'
			var cnt2=0, br=1;
			for(var key2 in sections[key]) {
				var fwidth=false; cnt2++;
				mykey = sections[key][key2];
				if (txtboite.hasOwnProperty(mykey)) {
					fwidth=txtboite[mykey].hasOwnProperty('width');
					elthtml=insert_txtboite(mykey, false, shrunk);
				} else if (dateboite.hasOwnProperty(mykey)) {
					fwidth=dateboite[mykey].hasOwnProperty('width');
					elthtml=insert_dateboite(mykey, false, shrunk);
				} else if (listboite.hasOwnProperty(mykey)) {
					fwidth=listboite[mykey].hasOwnProperty('width');
					elthtml=insert_listboite(mykey, false, shrunk);
				} else if (checkboite.hasOwnProperty(mykey)) {
					elthtml=insert_checkboite(mykey, additem=false);
				} else if (multiboite.hasOwnProperty(mykey)) {
					elthtml=insert_multiboite(mykey);
				} else if (areaboite.hasOwnProperty(mykey)) {
					elthtml=insert_areaboite(mykey);
				}
			// Output html
				if (cnt2>1 && (!fwidth || (fwidth && br==1))) myhtml+= '<br><br>';
				if (!fwidth) br=1; else if( fwidth && br==1) br=0;
				myhtml=myhtml+elthtml;
				if (fwidth) myhtml+='&nbsp;&nbsp;&nbsp;';
			}
			myhtml=myhtml+'<br/></div></td></tr>';
		}
		myhtml=myhtml+'</table>'
		$('#fields').append(myhtml)

		// Fill checkboxes && dropboxes with predefined vocabulary
		fill_elements (search=true);

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

		// Init array for sorting short view
		signsort = Array(shortview.length).fill(1);

	});

	</script>
</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_main ?>
<br>

<div id='loadmessage' class="loadmessage"><i>Loading external vocabulary</i>&nbsp;&nbsp;&nbsp;<img src="img/dots-loading.gif" width="30px"></div>

<form id="formsearch" name="formsearch" enctype="multipart/form-data" accept-charset="utf-8">
<section>
    <table border="0">
    <tr><td id="colitems" style="width: 25%; vertical-align: top;">
        <div id='fields' style="overflow-y: scroll; max-height: 800px;"></div>
    </td></tr>
    <tr><td style="vertical-align: top; height: 50px; padding: 20px;">
    <div style="float: left;">
        <h2 class="searchoptions">Search criterion</h2>
        <div style="padding-bottom: 10px;"><em>Indicate here if the fields you have entered are mandatory (search including all fields entered) or if they are optional (search including at least 1 field entered)</em></div>
        <div class="form-check">
          <label class="form-check-label">
            <input type="radio" class="form-check-input" name="operator" value="$and">Mandatory (AND)
          </label>
        </div>
		&nbsp;&nbsp;
        <div class="form-check">
          <label class="form-check-label">
            <input type="radio" class="form-check-input" name="operator" value="$or" checked>Optionnal (OR)
          </label>
        </div>
        <br/><br>
        <center><p>
            <input type="submit" value="Search" class="btn btn-primary"  style="width: 30%;"/>
            <input type="reset" value="Empty the form" class="btn btn-primary"  style="width: 30%;"onClick="$('#container').empty(); flip_all('off'); "/>
        </p></center>
    </div>
    </td></tr>
    <tr><td style="vertical-align: top; padding: 5px;">
        <div id="container" class="table-responsive" style="overflow-y: scroll;"></div>
    </td></tr>
    </table>
</section>
</form>

<br>

<script>
	$("[name='btn-search']").removeClass('btn-primary');  $("[name='btn-search']").addClass('menu-select');

	function final_loading() {
		bioportal_setup();
		active_autocomplete()
		$('#loadmessage').css('display','none');
	}

	$(window).on('load', function(){
		if ( $( window ).width()>maxWidth ) $('#colitems').prop("rowspan", 3);
		if($.active > 0)
			$(document).ajaxStop(function(){ $(document).unbind("ajaxStop"); window.setTimeout(final_loading(),300); });
		else
			final_loading();
		sendData();
	})

	$(window).on('resize', function(){
		if ( $( window ).width()>maxWidth ) {
			$('#colitems').prop("rowspan", 3);
		} else {
			$('#colitems').prop("rowspan", -1);
		}
	})

</script>
