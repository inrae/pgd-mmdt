<!DOCTYPE html>
<html lang="en" >

<head>
   <meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
     <title>Maggot - Search</title>
    <link rel="icon" href="favicon.ico" />

    <!-- CSS -->
    <link rel="stylesheet" href="css/bootstrap.min.css">
    <link rel="stylesheet" href="css/style.css">
    <link rel="stylesheet" href="css/bootstrap-toc.css">

    <!-- JS -->
    <script src="js/jquery.min.js"></script>
    <script src="js/utils.js"></script>
    <script src="js/formsearch.js"></script>
    <script src="js/autocomplete/bioportal.js"></script>
    <script src="js/autocomplete/voinrae.js"></script>

    <script type="text/javascript">
    var maxField = 10; //Input fields increment limitation
    var vartocomplete = [];
    function $_(id) { return document.getElementById(id) }

    $(document).ready(function() {
        // Load json from user
        $('.load_data').click(function() {
            var file = document.getElementById("selectFiles").files;
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
        sections = arrItems[1];    // sections which gathered one or severals variable
        checkboite = arrItems[3];  // list of the checkboxes along with their parameters
        listboite = arrItems[4];   // list of the dropboxes along with their parameters
        txtboite = arrItems[5];    // list of the input boxes along with their parameters
        areaboite = arrItems[6];   // list of the textarea boxes along with their parameters
        chkbxdico = arrItems[7];   // dictionnaries for checkboxes
        listdico = arrItems[8];    // dictionnaries for dropboxes

        // Insert HTML fields defined in arrItems
        cnt=0;
        myhtml='<table style="border:0; padding:0; max-width:1200px; width:100%">'
        for (var key in sections['labels']) {
            cnt++;
            myhtml=myhtml+'<tr><td style="width:10px"><img src="img/arrow-right.png" width="20px" onclick="flip_form(\'SECTION'+cnt+'\')" id="SECTION'+cnt+'img" class="link"></td><td><span class="sectiontitle">'+sections['labels'][key]+'</span></td></tr><tr><td></td><td><DIV id="SECTION'+cnt+'" style="display: none;">'
            fwidth=0; br=0;
            for(var key2 in sections[key]) {
                mykey = sections[key][key2];
                if (txtboite.hasOwnProperty(mykey)) {
                    fwidth=txtboite[mykey].hasOwnProperty('width');
                    if (fwidth) br=1;
                    elthtml=insert_txtboite(mykey, false);
                } else if (listboite.hasOwnProperty(mykey)) {
                    fwidth=listboite[mykey].hasOwnProperty('width');
                    if (fwidth) br=1;
                    elthtml=insert_listboite(mykey);
                } else if (checkboite.hasOwnProperty(mykey)) {
                    fwidth=0;
                    elthtml=insert_checkboite(mykey, additem=false);
                } else if (areaboite.hasOwnProperty(mykey)) {
                    fwidth=0;
                    elthtml=insert_areaboite(mykey);
                }
                if (fwidth==0 && br==1) { myhtml=myhtml+'<br/><br/>'; br=0;}
                myhtml=myhtml+elthtml;
                if (fwidth==1) myhtml=myhtml+'&nbsp;&nbsp;&nbsp;';
                if (fwidth==0 && br==0) myhtml=myhtml+'<br/>';
            }
            myhtml=myhtml+'<br/></div></td></tr>';
        }
        myhtml=myhtml+'</table>'
        $('#fields').append(myhtml)

        // Fill checkboxes && dropboxes with predefined vocabulary
        fill_elements (search=true);

    });

    </script>
</head>

<body>

<div class="entete">Metadata Management Tool for Dataset</div>
<br>

<div class="left-div">
    <div class="center-div">
        <div class="btn-group btn-group-lg" role="group">
          <a href="./" class="search_data"><button type="button" class="btn btn-outline-secondary mr-2" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-input">Metadata input</button></a>
          <a href="./search" class="search_data"><button type="button" class="btn btn-outline-secondary mr-2" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-search">Dataset research</button></a>
          <a href="./doc/about" class="search_data" target="_blank"><button type="button" class="btn btn-outline-info mr-2" data-toggle="button" aria-pressed="false" autocomplete="off">Documentation</button></a>
        </div>
    </div>
</div>

<section>
    <form id="formsearch" name="formsearch" enctype="multipart/form-data" accept-charset="utf-8">

        <div id='fields'></div>

        <br/>
        <h2 class="searchoptions">Search criterion</h2>
        <em>Indicate here if the fields you have entered are mandatory (search including all fields entered) or if they are optional (search including at least 1 field entered)</em>
        <br/><br/>
        <div class="form-check-inline">
          <label class="form-check-label">
            <input type="radio" class="form-check-input" name="operator" value="$and" checked>Mandatory (AND)
          </label>
        </div>
        <div class="form-check-inline">
          <label class="form-check-label">
            <input type="radio" class="form-check-input" name="operator" value="$or">Optionnal (Or)
          </label>
        </div>

        <br/><br/>

        <p id="res"></p>

        <center><p>
            <input type="submit" value="Search" class="btn btn-primary"  style="width: 30%;"/>
            <input type="reset" value="Empty the form" class="btn btn-primary"  style="width: 30%;"onClick="$('#container').empty();"/>
        </p></center>
    </form>
</section>

<a name="results"></a>
<div id="container" class="table-responsive"></div>

<br>
<hr>
<br>
<footer>
   <table><tr>
   <td><img src="./docs/logo.png"></td>
   <td style="text-align: bottom;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
   <td><img src="./img/logo_inrae.svg"></td>
   <td style="vertical-align: middle;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&copy; INRAE 2022&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
   </tr></table>
</footer>

<script>
    $("[name='btn-search']").css({'color': '#fff', 'background-color': '#6c757d', 'border-color': '#6c757d'})
</script>


</body>
</html>
