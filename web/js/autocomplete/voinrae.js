// VOINRAE API : https://consultation.vocabulaires-ouverts.inrae.fr/api/
// Example : keywords = ['data','report','simulation','model','image','script','omics','statistic','scientific','research','experiment','video','spatial', 'instrument' ]

var voinrae = [];

var get_terms_from_voinrae = function(keywords, varname)
{
    vocab='thesaurus-inrae'
    lang='en'
    type='skos%3AConcept'
    url='https://consultation.vocabulaires-ouverts.inrae.fr/rest/v1/search?vocab'+vocab+'&lang='+lang+'&type='+type+'&query='
    voinrae[varname] = {'url' : url, 'pageCount': 0, 'error' : '' };

    // Parses the json and retrieves all prefLabels.
    var successFunc = function(json) {
        $.each(json['results'], function (index, value) {
            if (!eval(varname).includes(value['prefLabel'], 0)) eval(varname).push(value['prefLabel']);
            //if (!eval(varname).includes(value['altLabel'], 0)) eval(varname).push(value['altLabel']);
        });
    }

    // Error function in case of API failure
    var errorFunc = function(XMLHttpRequest, textStatus, errorThrown) {
        voinra[varname]['error']=textStatus;
        alert('API VOINRAE: '+textStatus, errorThrown);
    }

    $.ajaxSetup({async:true});
    keywords.forEach(function(keyword){
        $.ajax({
           type: 'GET',  url: voinrae[varname]['url']+'*'+keyword+'*&maxhits=99&offset=0', 
           header: { 'accept': 'application/json', 'Access-Control-Allow-Origin': '*' },
           success: function(json) { voinrae[varname]['pagecount'] = voinrae[varname]['pagecount']+1; successFunc(json); },
           error: errorFunc
        });
    })
}
