// Test if a url exists, i.e. a javascript for example
function UrlExists(url)
{
    var http = new XMLHttpRequest();
    http.open('HEAD', url, false);
    http.send();
    return http.status!=404;
}

// Loads a javascript and triggers an action at the end of the load
function loadScript(url, callback)
{
    // Adding the script tag to the head
    var head = document.head;
    var script = document.createElement('script');
    script.type = 'text/javascript';
    script.src = url;
    script.async=false;

    // Then bind the event to the callback function.
    // There are several events for cross browser compatibility.
    script.onreadystatechange = callback;
    script.onload = callback;

    // Fire the loading
    head.appendChild(script);
}

// Load a user's json
function load_data(file)
{
    if (file.length <= 0) {
        alert("Please select a file in JSON format !");
        return false;
    }

    var json = new FileReader(); // Reader

    // Get the JSON-Schema file
    my_schema = '';
    $.ajaxSetup({async:false});
        $.getJSON("json/pgd-mmdt-schema.json", function (data) {
             my_schema=JSON.stringify(data);
        });
    $.ajaxSetup({async:true});

    var fr = new FileReader(); // Reader

    fr.onload = function(e) {
        var id = "";
        var tags = Object.keys(checkboite);
        var lists = Object.keys(listboite);
        var autotxt = Object.keys(txtboite);
        var autoarea = Object.keys(areaboite);
        var textarea = autotxt.concat(autoarea);

        // Validation based on JSON schema
        indata = this.result;
        resp='';
        $.ajaxSetup({async:false});
           $.get( "check", { json: indata } ).done(function(outdata) { console.log(outdata); resp=JSON.parse(outdata); });
        $.ajaxSetup({async:true});

        // If no errors
        if (resp.errors.length==0) {
        //ok=1; if (ok==1) {
            var result = JSON.parse(e.target.result);
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
                        } else {
                            alert('Please re-enter this value in the corresponding text field of the form : '+id_info);
                        }
                    }
                    // Lists
                    if (lists.includes(item)) {
                        var element = document.getElementById(item+'_select')
                        if (element != null) element.value = obj;
                    }
                    // Text
                    if (textarea.includes(item)) {
                        document.getElementById(item).value = obj;
                    }
                }
            });
            // Resources
            res=result['resources'];
            for( var i = 1; i <= res.length; ++i ) {
                 if(i>1) add_bouton_r();
                 document.getElementsByName('resource-f1-'+i)[0].value=res[i-1]['datatype'];
                 document.getElementsByName('resource-f2-'+i)[0].value=res[i-1]['description'];
                 document.getElementsByName('resource-f3-'+i)[0].value=res[i-1]['location'];
            }
        } else {
        // If errors
            message = 'ERROR: the uploaded file is not a valid JSON file.';
            for (line of resp.errors) message = message + "\n"+line
            alert(message);
            return false;
        }
    }

    fr.readAsText(file.item(0));
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
}


/* Autocompletion */
function autocomplete(inp, arr)
{
    /*the autocomplete function takes two arguments,
     the text field element and an array of possible autocompleted values:*/
    var currentFocus;
    /*execute a function when someone writes in the text field:*/
    inp.addEventListener("input", function(e) {
       var a, b, i, val = this.value;
       /*close any already open lists of autocompleted values*/
       closeAllLists();
       if (!val || val.length < 3) { return false;}
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

function switchTo(elementname, state){
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
function flipDisplay(elementname){
    element = $_(elementname);
    if (element)
        if (element.style.display == 'inline') switchTo(elementname, 'off');
        else                                   switchTo(elementname, 'on');
}
function switch_tree(id,n,state){
   for (var i=1;i<=n;i++) {
       switchTo(id+i, state);
   }
}
function flip_form(div){
    flipDisplay(div);
    divimg=div+'img';
    $_(divimg).onclick=function(){ flipDisplay(div); }
}

//----------------------------------------
/* insert Formular elements */
//----------------------------------------

function insert_checkboite (identif_boite, additem=true, req=false)
{
    var divclass=asterisk=plh='';
    if (req) asterisk ='&nbsp;<span style="color:red;">(*)</span>'
    var myfieldHTML =
        '<div style="padding:4px; border:4px solid #ebebeb;">\
            <div style="padding:3px; background-color:#fff;">\
                <div id="'+identif_boite+'" class="form-group '+identif_boite+'">\
                    <label for="name" class="labeldiv">'+checkboite[identif_boite]['titre']+'</label>'+asterisk+'\
                    <fieldset></fieldset>\
                </div>';

    if (additem && checkboite[identif_boite]['open']) {
        if (checkboite[identif_boite].hasOwnProperty('autocomplete')) {
            vartocomplete[vartocomplete.length]= { variable : identif_boite+'-1', dico: checkboite[identif_boite]['autocomplete'], listener: 0 };
            url_script='js/autocomplete/'+checkboite[identif_boite]['autocomplete']+'.js'
            if (UrlExists(url_script)) loadScript(url_script, active_autocomplete)
            divclass='class="autocomplete"';
            plh='placeholder="enter the first three letters"';
        }
        checkboite[identif_boite]['compteur'] =  2;
        myfieldHTML += '\
                <br><label for="select" class="labeldiv">OR enter an item (if missing in the above options) :</label>\
                <div class="field_wrapper '+identif_boite+'">\
                    <div '+divclass+' style="width:100%">\
                        <input class="form-control form-control-sm '+identif_boite+'" type="text" name="'+identif_boite+'-1" id="'+identif_boite+'-1" '+plh+'value=""/>\
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

function insert_listboite (identif_boite, req=false)
{
    var divclass='class="'+identif_boite+'"';
    if (listboite[identif_boite].hasOwnProperty('width')) { mywidth = 'style="width:'+listboite[identif_boite]['width']+'"'; divclass='class="form-group "'+identif_boite+'" '+mywidth; }
    var asterisk=''; if (req) asterisk ='&nbsp;<span style="color:red;">(*)</span>'
    var myfieldHTML =
        '<div '+divclass+'id="'+identif_boite+'">\
            <label for="name" class="labeldiv">'+listboite[identif_boite]['titre']+'</label>'+asterisk+'\
            <fieldset>\
                <select id="'+identif_boite+'_select" name="'+identif_boite+'" class="form-control form-control-sm '+identif_boite+'"></select>\
            </fieldset>\
        </div>';
    return myfieldHTML;
}

function insert_txtboite (identif_boite, req=false)
{
    var divclass=asterisk=mywidth=ac=plh='';
    if (txtboite[identif_boite].hasOwnProperty('width'))
        mywidth = 'style="width:'+txtboite[identif_boite]['width']+'"';
    if (txtboite[identif_boite].hasOwnProperty('autocomplete')) {
        vartocomplete[vartocomplete.length]= { variable : identif_boite, dico: txtboite[identif_boite]['autocomplete'], listener: 0 };
        url_script='js/autocomplete/'+txtboite[identif_boite]['autocomplete']+'.js'
        if (UrlExists(url_script)) loadScript(url_script, active_autocomplete)
        ac='autocomplete ';
        plh='placeholder="enter the first three letters"';
    }
    divclass='class="'+ac+'form-group" '+mywidth;
    if (req) asterisk ='&nbsp;<span style="color:red;">(*)</span>'
    var myfieldHTML =
        '<div '+divclass+'>\
            <label for="name" class="labeldiv">'+ txtboite[identif_boite]['titre'] +'</label>'+asterisk+'\
            <input class="form-control form-control-sm solo" type="text" name="'+ identif_boite +'" id="'+ identif_boite +'" '+ plh  +mywidth +'"/>\
        </div>';
    return myfieldHTML;
}

function insert_areaboite (identif_boite, req=false)
{
    var asterisk=''; if (req) asterisk='&nbsp;<span style="color:red;">(*)</span>'
    var myfieldHTML =
        '<div>\
            <label for="'+ identif_boite +'" class="labeldiv">'+ areaboite[identif_boite]['titre'] +'</label>'+asterisk+'\
            <textarea class="form-control form-control-sm" name="'+ identif_boite +'" rows="'+ areaboite[identif_boite]['rows'] +'" cols="'+ areaboite[identif_boite]['cols'] +'" id="'+ identif_boite +'"></textarea>\
        </div>';
    return myfieldHTML;
}

function add_bouton (chk_box_name)
{
    var cur_cpt = checkboite[chk_box_name]['compteur'];
    var targetDOM = '.field_wrapper.' + chk_box_name;

    if(cur_cpt < maxField){
        cur_cpt++; //Increment counter
        checkboite[chk_box_name]['compteur'] =  cur_cpt;
        var divclass=plh='';
        if (checkboite[chk_box_name].hasOwnProperty('autocomplete')) {
            vartocomplete[vartocomplete.length]= { variable : chk_box_name+'-'+ cur_cpt, dico: checkboite[chk_box_name]['autocomplete'], listener: 0 };
            divclass='class="autocomplete"';
            plh='placeholder="enter the first three letters"';
        }

        var fieldHTML = '<div '+divclass+' style="width:100%"><br/><input type="text" class="form-control form-control-sm '+ chk_box_name +'" name="'+ chk_box_name +'-'+ cur_cpt +'" id="'+ chk_box_name +'-'+ cur_cpt +'" '+ plh +' value=""/><a href="javascript:void(0);" class="remove_button '+ chk_box_name +'"><button type="button" class="btn btn-danger btn-xs" id="addrow"><i class="fa fa-plus-circle" aria-hidden="true"></i> Remove the line</button></a></div>';
        $(targetDOM).append(fieldHTML); //Add html
        active_autocomplete()
    }
}

// Resources
function insert_resource(id, arrTypes)
{
    opthml=''; for (key in arrTypes) opthml=opthml+'<option value="'+arrTypes[key]+'">'+arrTypes[key]+'</option>';
    var myfieldHTML =
    '<table class="resource" width="100%"><tr><td width="200px"><span class="labeldiv">Type</span></td><td><select id="resouce_select'+id+'" name="resource-f1-'+id+'" class="form-control form-control-sm resource">'+opthml+'</select></td></tr><tr><td><span class="labeldiv">Description</span></td><td><input class="form-control form-control-sm resource" type="text" name="resource-f2-'+id+'" value=""><td></tr><tr><td><span class="labeldiv">Location</span></td><td><input class="form-control form-control-sm resource" type="text" name="resource-f3-'+id+'" value=""><td></tr></table>';
    return myfieldHTML;
}

function insert_resources(id)
{
   var myfieldHTML =
   '<div style="padding:4px; border:4px solid #ebebeb;"><div style="padding:3px; background-color:#fff;"><div class="resource_wrapper"><div>'+insert_resource(id, listdico['resources'])+'<div><a href="javascript:void(0);" class="add_button_r resource" title="Add resource"><button type="button" class="btn btn-primary btn-xs" id="add_button_r"><i class="fa fa-plus-circle" aria-hidden="true"></i> Add a resource</button></a></div></div></div></div></div><br/>';
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
                                    <label class="form-check-label" for="'+ key + obj[i] +'">\
                                    <input type="checkbox" name="'+ key + '-' + localcpt + '" value="'+ obj[i] +'" class="form-check-input '+ key +'" id="'+ idobj +'">\
                                    '+ obj[i] +'</label></div>';
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

// Activate the all Autocomplete if defined
function active_autocomplete()
{
    if (vartocomplete.length>0) {
        for( var i = 0; i < vartocomplete.length; ++i )
           if (vartocomplete[i]['listener']==0 && vartocomplete[i]['dico'].length<15 && vartocomplete[i]['dico'].search("[,/?()=:;@$.]")<0)
              try {
                 autocomplete(document.getElementById(vartocomplete[i]['variable']), eval(vartocomplete[i]['dico']));
                 vartocomplete[i]['listener']=1;
              } catch(e) {}
    }
}

