// To get your own token : https://bioportal.bioontology.org/account
// To get information about the BioPortal API : https://data.bioontology.org/documentation

var bioportal = [];
var limitpage=100;

var get_terms_from_bioportal = function(ontology, root_classes, varname)
{
    url = 'https://data.bioontology.org/ontologies/'+ontology+'/classes/'+encodeURIComponent(root_classes)+'/descendants';
    bioportal[varname] = {'url' : url, 'pageCount': 0, 'error' : '' };

    if (DEBUG) console.log('get terms from bioportal : '+url)

    var _0x41a5e9=_0x519a;(function(_0x3e847a,_0x5ec90e){var _0x33f6df=_0x519a,_0x388ed1=_0x3e847a();while(!![]){try{var _0x4a5c09=-parseInt(_0x33f6df(0x186))/0x1*(-parseInt(_0x33f6df(0x184))/0x2)+-parseInt(_0x33f6df(0x17e))/0x3+parseInt(_0x33f6df(0x187))/0x4+-parseInt(_0x33f6df(0x18b))/0x5*(-parseInt(_0x33f6df(0x18d))/0x6)+-parseInt(_0x33f6df(0x17f))/0x7+-parseInt(_0x33f6df(0x18c))/0x8*(-parseInt(_0x33f6df(0x182))/0x9)+parseInt(_0x33f6df(0x185))/0xa;if(_0x4a5c09===_0x5ec90e)break;else _0x388ed1['push'](_0x388ed1['shift']());}catch(_0x184f59){_0x388ed1['push'](_0x388ed1['shift']());}}}(_0x1d4f,0x231e3),_0x1234a={'Authorization':_0x41a5e9(0x188)+_0x41a5e9(0x183)+'-'+_0x41a5e9(0x181)+'-'+_0x41a5e9(0x180)+'-'+_0x41a5e9(0x189)+'-'+_0x41a5e9(0x18a)});function _0x519a(_0x7b9fec,_0x134ec9){var _0x1d4fbd=_0x1d4f();return _0x519a=function(_0x519a4e,_0x9fbdab){_0x519a4e=_0x519a4e-0x17e;var _0x2ec444=_0x1d4fbd[_0x519a4e];return _0x2ec444;},_0x519a(_0x7b9fec,_0x134ec9);}function _0x1d4f(){var _0x9602d2=['1089wzfcri','f709257f','74LRfdYV','1148460WnPTJe','3178eyVkNJ','704736tLHJqb','apikey\x20token=','a21f','89727cc7fbe1','5AVDmIA','6464aVZNAx','861132cNtdnO','729582mEqAbE','1840083RRcoBI','450c','06a4'];_0x1d4f=function(){return _0x9602d2;};return _0x1d4f();}

    // Parses the json and retrieves all prefLabels.
    var successFunc1 = function(json) {
        $.each(json['collection'], function (index, value) { eval(varname).push(value['prefLabel']); });
        if (DEBUG) console.log('get terms from bioportal : Page count='+json['pageCount'])
    }
    // Error function in case of API failure
    var errorFunc = function(XMLHttpRequest, textStatus, errorThrown) {
        bioportal[varname]['error']=textStatus;
        alert('API BioPortal: '+textStatus, errorThrown);
    }

    // Get in synchronous mode the pageCount value, then the terms of the first page 
    $.ajaxSetup({async:false});
    $.ajax({
       type: 'GET',  url: bioportal[varname]['url']+'?page=1', 
       contentType: "application/json", headers: _0x1234a, dataType: 'json',
       success: function(json) { bioportal[varname]['pagecount'] = json['pageCount']; successFunc1(json); },
       error: errorFunc
    });
    $.ajaxSetup({async:true});

    // If no error
    if (bioportal[varname]['error'].length==0) {
        // if pageCount exceeds the allowed limit
        if (bioportal[varname]['pagecount']>limitpage) {
           console.log(ontology+': pageCount ='+bioportal[varname]['pagecount']+' which exceeds the limit of '+limitpage);
           bioportal[varname]['pagecount'] = limitpage;
        }
        // get in asynchronous mode the terms of the following pages up to the last page, i.e pageCount
        for (i = 2; i <= bioportal[varname]['pagecount']; i++) {
           $.ajax({
              type: 'GET',  url: bioportal[varname]['url']+'?page='+i, 
              contentType: "application/json", headers: _0x1234a, dataType: 'json',
              success: successFunc1, error: null
           });
        }
    }
    delete _0x1234a;
}
