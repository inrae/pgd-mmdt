<!DOCTYPE html>
<!-
  Copyright: (c) INRAE - 2022-2023
->
<html lang="en" >
<head>
	<title>Maggot - Search</title>
	<?= $HEADMAIN ?>

	<style>::-webkit-scrollbar{ width: 0px; }</style>
	<script src="js/search.js"></script>
	<script type="text/javascript">
	var DEBUG = <? echo isset($_GET['debug']) ? $_GET['debug'] : 0 ?>;
	var ADVS = <? echo isset($_GET['advsearch']) ? $_GET['advsearch'] : 0 ?>;
	var advdisp = ADVS ? '' : 'none' ;
	var simpledisp = ADVS ? 'none' : '' ;
	var AJAXWAIT = "<? echo isset($_GET['wait']) ? $_GET['wait'] : $AJAXWAIT ?>";
	var arrItems;
	var maxField = 10; //Checkboxes increment limitation
	var maxWidth=1200;
	var cvdir=<?php echo "'".$cvdir."'" ?>;
	var signsort = Array();

	function $_(id) { return document.getElementById(id) }

	advClick = function () {
		$('.advsearch').css('display', '');
		$('.simplesearch').css('display', 'none');
		sendData(type=1);
	}

	simpleClick = function () {
		$('.advsearch').css('display', 'none');
		$('.simplesearch').css('display', '');
		search2 = $("#search2").val()
		if (search2.length) {
			sendData(type=2);
		} else {
			$('#formsearch').trigger("reset");
			sendData(type=1);
		}
	}

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

		$(".advsearch").css("display", advdisp);
		$(".simplesearch").css("display", simpledisp);
	});

	</script>
</head>

<body style="zoom: <?= $ZOOMWP ?>">

<?= $header_main ?>
<br>

<div id='loadmessage' class="loadmessage"><i>Loading external vocabulary</i>&nbsp;&nbsp;&nbsp;<img src="img/dots-loading.gif" width="30px"></div>

<form id="formsearch2" name="formsearch2" enctype="multipart/form-data" accept-charset="utf-8">
<section>
	<table class="simplesearch" style="width: 100%" border="0">
	<tr><td colspan=2><span class="labeldiv">Selection by Keywords &nbsp;<a href="<?= $maggot_doc_url.'tutorial/search/' ?>" target="doc"><i class="bi-question-octagon-fill" style="color: rgb(27, 130, 193); font-size: 12px;" onmouseover="$(this).css('font-size','18px');" onmouseleave="$(this).css('font-size','12px');"></i></a></span></td></tr>
	<tr><td style="width: 77%">
		<div class="form-group2" style="width: 100%">
			<input placeholder="list of keywords" class="form-control form-control-sm solo " type="text" name="search2" id="search2" style="width: 100%">
		</div>
	</td><td style="width: 23%">
		<div class="form-group2 ">
			<button class="btn btn-primary bi-search" type="submit" aria-label="Search">&nbsp;Search</button>
			&nbsp;&nbsp;<a class="textlink" onclick="advClick();">Go to Advanced search</a>
		</div>
	<tr><td colspan=2>
        <div class="form-check">
          <label class="form-check-label">
            <input type="radio" class="form-check-input" name="operator2" value="and" checked>Mandatory (AND)
          </label>
        </div>
		&nbsp;&nbsp;
        <div class="form-check">
          <label class="form-check-label">
            <input type="radio" class="form-check-input" name="operator2" value="or">Optionnal (OR)
          </label>
        </div>
	</td></tr>
	<tr><td colspan=2><span class="labeldiv">&nbsp;</span></td></tr></table>
</section>
</form>

<form id="formsearch" name="formsearch" enctype="multipart/form-data" accept-charset="utf-8">
<section>
    <table border="0" style="width: 100%">
    <tr><td class="advsearch" id="colitems" style="width: 25%; vertical-align: top;">
        <div id='fields' style="overflow-y: scroll; max-height: 800px;"></div>
    </td></tr>
    <tr><td class="advsearch" style="vertical-align: top; height: 50px; padding: 20px;">
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
            <button type="submit" class="btn btn-primary bi-search"  style="width: 20%;" />&nbsp;Search</button>
            <button type="reset" class="btn btn-primary bi-trash"  style="width: 20%;" onClick="$('#container').empty(); flip_all('off'); "/>&nbsp;Empty the form</button>
			&nbsp;&nbsp;<a class="textlink" onclick="simpleClick();">Go to Simple search</a>
        </p></center>
    </div>
    </td></tr>
    <tr><td style="vertical-align: top; padding: 5px;">
        <div id="container" class="table-responsive" style="overflow-y: scroll;"></div>
    </td></tr>
    </table>
	<input type="hidden" id="sendflag" name="sendflag" value="0">
</section>
</form>

<br>

<script>
	$("[name='btn-search']").removeClass('btn-primary');  $("[name='btn-search']").addClass('menu-select');

	function final_loading(){
		if (DEBUG) console.log('--- BEGIN final_loading ...')
		bioportal_setup();
		active_autocomplete()
		$('#loadmessage').css('display','none');
		if (DEBUG) console.log('--- END final_loading ...')
	}

	function differ_ajaxtest(){
		if($.active > 0) {
			if (DEBUG) console.log('Wait for AJAX to finish ...')
			$(document).ajaxStop(function(){ $(document).unbind("ajaxStop"); setTimeout(final_loading,300); });
		}
		else {
			if (DEBUG) console.log('Wait '+AJAXWAIT+' ms')
			setTimeout(final_loading,AJAXWAIT);
		}
	}
	
	$(window).on('load', function(){
		setTimeout(differ_ajaxtest, 1000)
		if ( $( window ).width()>maxWidth ) $('#colitems').prop("rowspan", 3);
		sendData(type=1); $('#sendflag').val(0);
	})

	$(window).on('resize', function(){
		if ( $( window ).width()>maxWidth ) {
			$('#colitems').prop("rowspan", 3);
		} else {
			$('#colitems').prop("rowspan", -1);
		}
	})

</script>
