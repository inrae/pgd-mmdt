(function() {
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
            if (/^datatype-\d/.test(name) == true || /^valorisation-\d/.test(name)) { continue; }
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
                if( name && value.length>0) obj[ name ] = value;
            }
        }
        // Select
        var elements = form.querySelectorAll( "select" );
        for( var i = 0; i < elements.length; ++i ) {
            var element = elements[i];
            var name = element.name;
            if (!name.includes('resource-f1-')) {
                var value = element.value;
                if( name && value.length>0) obj[ name ] = value;
            }
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
            obj[key] = listData;
        }
        // Resources
        R=formulaire.querySelectorAll( 'table.resource' )
        if (R.length>0) {
            var listData = [];
            for( var i = 0; i < R.length; ++i ) {
                var res = {"datatype": R[i].querySelector('select').value, "description": R[i].querySelectorAll('input')[0].value, "location": R[i].querySelectorAll('input')[1].value}
                if (res['description'].length>0)
                     listData.push(res);
            }
            obj['resources'] = listData;
        }

        return JSON.stringify( obj );
    }

    function check_required(json, required)
    {
        var ret=true;
        obj=JSON.parse(json);
        keys = Object.keys(obj);
        var errmsg="WARNING:\n\nThe following field(s):\n\n";
        Object.keys(required).forEach(key => {
          if (!keys.includes(key)) {
               errmsg=errmsg+"* "+required[key]+"\n"
               ret=false;
           }
        });
        if (!ret) alert(errmsg+"\nmust be defined");
        return ret;
    }

    function saveFormAsTextFile(json)
    {
        var textToSaveAsBlob = new Blob([json], {type:"text/plain"});
        var textToSaveAsURL = window.URL.createObjectURL(textToSaveAsBlob);
        var timestamp = Math.round(+new Date() / 1000);
        var fileNameToSaveAs = 'PGD_'+timestamp+'.json';
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

    // Generation du fichier au format json : clic sur bouton "Enregistrer dans un fichier"
    document.addEventListener( "DOMContentLoaded", function()
    {
        var form = document.getElementById("formulaire");
        var output = document.getElementById("output");
        form.addEventListener( "submit", function( e ) {
            e.preventDefault();
            var jsonform = toJSONString( this );
            if (check_required(jsonform, required))
                saveFormAsTextFile(jsonform);
        }, false);

    });
})();
