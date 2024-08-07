// Deprecated - Instead use the typeahead mechanism 
var skosmos = [];

var get_terms_from_skosmos = function(apiurl, radicals, varname, lang='en', parent='', group='', asyncmode=true)
{
	type='skos:Concept'
	maxhits=2000
	url=apiurl+'/search?lang='+lang+'&type='+type
	if (parent.length>0) url = url + '&parent='+parent
	if (group.length>0) url = url + '&group='+group
	url = url + '&query='
	skosmos[varname] = {'url' : url, 'pageCount': 0, 'error' : '', 'count': 0 };

	if (DEBUG) console.log('get terms from skosmos : '+url)

	// Parses the json and retrieves all prefLabels.
	var successFunc = function(json) {
		$.each(json['results'], function (index, value) {
			if (value.hasOwnProperty('prefLabel') && !eval(varname).includes(value['prefLabel'], 0)) eval(varname).push(value['prefLabel']);
			if (value.hasOwnProperty('altLabel') && !eval(varname).includes(value['altLabel'], 0)) eval(varname).push(value['altLabel']);
		});
	}
	
	// Error function in case of API failure
	var errorFunc = function(XMLHttpRequest, textStatus, errorThrown) {
		voinra[varname]['error']=textStatus;
		alert('API SKOSMOS: '+textStatus, errorThrown);
	}
	
	$.ajaxSetup({async:asyncmode});
	
	if ((parent.length>0 || group.length>0) && radicals.length==0)
		$.ajax({
			type: 'GET',  url: skosmos[varname]['url']+'*&maxhits='+maxhits+'&offset=0', 
			header: { 'accept': 'application/json', 'Access-Control-Allow-Origin': '*' },
			success: function(json) { skosmos[varname]['pageCount']++; successFunc(json); skosmos[varname]['count']=eval(varname).length; },
			error: errorFunc
		});
	else 
		// Get all terms for each radical
		radicals.forEach(function(keyword){
			$.ajax({
				type: 'GET',  url: skosmos[varname]['url']+'*'+keyword+'*&maxhits='+maxhits+'&offset=0', 
				header: { 'accept': 'application/json', 'Access-Control-Allow-Origin': '*' },
				success: function(json) { skosmos[varname]['pageCount']++; successFunc(json); skosmos[varname]['count']=eval(varname).length; },
				error: errorFunc
			});
		})
}
