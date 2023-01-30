<!DOCTYPE html>
<html lang="en" >

<head>
    <meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
    <title>Maggot - Main</title>
    <link rel="icon" href="favicon.ico" />

    <!-- CSS -->
    <link rel="stylesheet" href="css/bootstrap.min.css">
    <link rel="stylesheet" href="css/style.css">

    <!-- JS -->
    <script src="js/jquery.min.js"></script>
    <script src="js/utils.js"></script>
    <script src="js/index.js"></script>
    <script src="js/autocomplete/bioportal.js"></script>
    <script src="js/autocomplete/voinrae.js"></script>

    <script type="text/javascript">
    var url_schema = '<?php echo $url_schema; ?>';
    var maxField = 10; //Checkboxes increment limitation
    var maxResource = 20; //Resources increment limitation
    var vartocomplete = [];
    var required = { 'title':'', 'description':'' };
    var nomenu= <?php if(isset($_GET['nomenu'])){ echo "true"; } else { echo "false"; } ?> ;

    $(document).ready(function() {

        function add_section_name(section) {
            $('#fields').append('<br><table><tr><td><h3>'+section+'</h3></td>'+
                                '<td valign="top">&nbsp;<a href="doc/meta#'+section+'" target="doc">'+
                                '<img src="img/help.png" width="12px"></a></td></tr></table>')
        }

        // Load json from user
        $('.load_data').click(function() {
            var file = document.getElementById("selectFiles").files;
            reset_form();
            load_data(file);
        });

        // Load 'json/options.json' into the arrItems array
        var arrItems;
        $.ajaxSetup({async:false});
            $.getJSON("json/options.json", function (data) {
                arrItems = [];
                $.each(data, function (index, value) {
                    arrItems.push(value);
                });
            });
        $.ajaxSetup({async:true});

        // Get the different dictionnaries
        sections = arrItems[0];    // sections which gathered one or severals variable
        checkboite = arrItems[3];  // list of the checkboxes along with their parameters
        listboite = arrItems[4];   // list of the dropboxes along with their parameters
        txtboite = arrItems[5];    // list of the input boxes along with their parameters
        areaboite = arrItems[6];   // list of the textarea boxes along with their parameters
        chkbxdico = arrItems[7];   // dictionnaries for checkboxes
        listdico = arrItems[8];    // dictionnaries for dropboxes

        // Insert HTML fields defined in arrItems
        for (var key in sections['labels']) {
            if (key=='resources') continue;
            add_section_name(sections['labels'][key])
            fwidth=0; br=0;
            // Sections
            for(var key2 in sections[key]) {
                mykey = sections[key][key2];
                frequired = required.hasOwnProperty(mykey)
            // txtboite
                if (txtboite.hasOwnProperty(mykey)) {
                    fwidth=txtboite[mykey].hasOwnProperty('width');
                    if (frequired) required[mykey]=txtboite[mykey]['titre'];
                    if (fwidth) br=1;
                    myhtml=insert_txtboite(mykey, req=frequired);
            // listboite
                } else if (listboite.hasOwnProperty(mykey)) {
                    fwidth=listboite[mykey].hasOwnProperty('width');
                    if (frequired) required[mykey]=listboite[mykey]['titre'];
                    if (fwidth) br=1;
                    myhtml=insert_listboite(mykey, req=frequired);
            // checkboite
                } else if (checkboite.hasOwnProperty(mykey)) {
                    fwidth=0;
                    if (frequired) required[mykey]=checkboite[mykey]['titre'];
                    myhtml=insert_checkboite(mykey, additem=!nomenu, req=frequired);
                    checkboite[mykey]['compteur'] = 1;
            // areaboite
                } else if (areaboite.hasOwnProperty(mykey)) {
                    fwidth=0;
                    if (frequired) required[mykey]=areaboite[mykey]['titre'];
                    myhtml=insert_areaboite(mykey, req=frequired);
                }
            // Output html 
                if (fwidth==0 && br==1) { $('#fields').append('<br/><br/>'); br=0;}
                $('#fields').append(myhtml);
                if (fwidth==1) $('#fields').append('&nbsp;&nbsp;&nbsp;');
                if (fwidth==0 && br==0) $('#fields').append('<br/>');
            }
            $('#fields').append('<br/>');
        }

        // Add Listeners
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

        // Fill checkboxes && dropboxes with predefined vocabulary
        fill_elements ();

        // "Resources" section 
        if (sections['labels'].hasOwnProperty('resources')) {
            id=1;
            listboite['resources'] = { 'compteur': id };
            add_section_name(sections['labels']['resources'])
            $('#fields').append(insert_resources(id))
    
            $('.add_button_r').click( function(e) { add_bouton_r(); })
            $('.resource_wrapper').on('click', '.remove_button_r', function(e){
                e.preventDefault();
                content = $(this).parent('div').find( "input" ).val();
                if (content.length==0 || confirm("Suppression confirmation:\nEither OK or Cancel.")) {
                    $(this).parent('div').remove(); //Remove resource html
                    listboite['resources']['compteur']--; //Decrement resource counter
                }
            });
		    
            $('#fields').append('<br/>');
        }
    });

    </script>
</head>

<body>

<div class="sticky-image-wrapper">
    <table><tr class="arrowtbl"><td class="arrowtbl">
       <a onclick="window.scrollTo({ top: 0, behavior: 'smooth' });"><img src="img/arrow-up-grey.png" width="25px" height="25px" /></a>
    </td></tr><tr class="arrowtbl"><td class="arrowtbl">
       <a onclick="window.scrollTo(0,document.body.scrollHeight);"><img src="img/arrow-down-grey.png" width="25px" height="25px" /></a>
    </td></tr></table>
</div>

<div class="header"><div class="progress-container"><div class="progress-bar" id="myBar"></div></div></div>

<div class="entete">Metadata Management Tool for Dataset</div>
<br>

<?php if( ! isset( $_GET['nomenu'] ) )
echo '
<div class="left-div">
    <div class="center-div">
        <div class="btn-group btn-group-lg" role="group">
          <a href="./" class="search_data"><button type="button" class="btn btn-outline-secondary mr-2" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-input">Metadata input</button></a>
          <a href="./search" class="search_data"><button type="button" class="btn btn-outline-secondary mr-2" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-search">Dataset research</button></a>
          <a href="./doc/about" class="search_data" target="_blank"><button type="button" class="btn btn-outline-info mr-2" data-toggle="button" aria-pressed="false" autocomplete="off">Documentation</button></a>
        </div>
    </div>
</div>
'; ?>

<section>
    <form id="formulaire" enctype="multipart/form-data" autocomplete="off" action="#" method="post">

        <h2>1/ Upload a JSON (Optionnal)</h2>
        <div class="input-group mb-3">
            <div class="input-group-prepend">
                <span class="input-group-text">Select a JSON file</span>
            </div>
            <div class="custom-file">
                <input type="file" class="custom-file-input" id="selectFiles">
                <label class="custom-file-label" id="inputGroupFile01" for="inputGroupFile01"></label>
            </div>
        </div>
        <a href="javascript:void(0);" class="load_data"><button type="button" class="btn btn-success btn-xs" id="load">Upload</button></a>

        <br/><br/><br/><br/>
        <h2>2/ Enter/Modify the Metadata for the Dataset</h2>

        <div id='fields'></div>

        <p><span style="color:red;">(*)</span>&nbsp;<i>mandatory fields</i></p>
    </form>
</section>

<!-- Save as JSON -->
<div id="saveForm" class="saveForm" enctype="multipart/form-data" autocomplete="off" action="#" style="display: none;"> 
   <strong>Download as a JSON File</strong>
   <form id ="saveform" name="saveform" >
       <br><label>Filename:</label>&nbsp;&nbsp;<b>PGD_</b>&nbsp;<input id="jsonName" name="jsonName" type="text">&nbsp;<b>.json</b><br><br>
       <span><center>
            <input type="submit" value="Save" class="btn btn-primary" />
            <input type="reset" value="Cancel" class="btn btn-primary"/>
        </center></span>
    </form>
</div><!-- /div Save as JSON -->

<form id="submitForm" enctype="multipart/form-data" autocomplete="off" action="#" method="post">
    <center><p>
       <input type="submit" value="Download as a JSON file" class="btn btn-primary" style="width: 30%;" />
       <input type="reset" value="Empty the form" class="btn btn-primary" style="width: 30%;" onclick="window.location.reload();"/>
    </p></center>
</form>

<br/>
<hr>

<footer>
    <table><tr>
    <td><img src="./docs/logo.png"></td>
    <td style="text-align: bottom;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
    <td><img src="./img/logo_inrae.svg"></td>
    <td style="vertical-align: middle;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&copy; INRAE 2022&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
    </tr></table>
</footer>

<script>
    /* Montrer le nom du fichier selectionner pour chargement : https://www.codeply.com/p/LtpNZllird */
    document.querySelector('.custom-file-input').addEventListener('change',function(e){
       var fileName = document.getElementById("selectFiles").files[0].name;
       var nextSibling = e.target.nextElementSibling
       nextSibling.innerText = fileName
    });
    $("[name='btn-input']").css({'color': '#fff', 'background-color': '#6c757d', 'border-color': '#6c757d'})

    // When the user scrolls the page, execute myFunction
    window.onscroll = function() {scrollBar()};
    function scrollBar() {
       var winScroll = document.body.scrollTop || document.documentElement.scrollTop;
       var height = document.documentElement.scrollHeight - document.documentElement.clientHeight;
       var scrolled = (winScroll / height) * 100;
       document.getElementById("myBar").style.width = scrolled + "%";
    }
</script>
</body>
</html>
