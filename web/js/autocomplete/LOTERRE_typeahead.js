// -- skosmos type --

// NOTE : You can copy this script to use it for another SKOSMOS portal (cf https://skosmos.org/).
// Simply 1) change all 'LOTERRE' occurrences to the name of the new portal; 2) change the URL, logo and options according to the new portal.

// Variables used only in this script.
let LOTERRE_api = 'https://loterre.istex.fr/rest/v1'
let LOTERRE_options = 'lang=en&type=skos:Concept&parent=&group='
let LOTERRE_logo = 'https://loterre.istex.fr/images//istex.png'
let LOTERRE_limit = 1000

// this function must be named as <ws>_typeahead, where <ws> must correspond to the name of the web service
// specified  in the 'ws' attribute in the 'features' column in the line relating to the field concerned.

// idName : the magggot field name 
// thesaurus : name of the thesaurus
var LOTERRE_typeahead = function (idName, thesaurus)
{
	$('#LOTERRE-'+idName+' .typeahead').typeahead({ hint: true,	highlight: true, minLength: 3 },
	{
		limitget: LOTERRE_limit,
		limitview : LOTERRE_limit,
		async: true,
		source: function (query, processSync, processAsync) {
			url = LOTERRE_api
			if (thesaurus != 'all')	url = url + '/' + thesaurus
			url = url+'/search?'+LOTERRE_options+'&query=*'+encodeURIComponent(query)+'*'
			if (DEBUG) console.log('GET '+url)
			return $.ajax({
				url: url,
				type: 'GET',
				dataType: 'json',
				success: function (json) {
					return processAsync(json.results);
				}
			});
		},
		templates: {
			header: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="'+LOTERRE_logo+'" style="width:60px;"/>&nbsp;&nbsp;'+
				'<b><i><font color="grey" size=-1>&nbsp;'+thesaurus+'</font></i></b>',
				'</div>'
			].join('\n'),
			pending: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="'+LOTERRE_logo+'" style="width:60px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp;... wait ...</i></b>',
				'</div>'
			].join('\n'),
			empty: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="'+LOTERRE_logo+'" style="width:60px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp; this term seems not exist !! </i></b>',
				'</div>'
			].join('\n'),
			suggestion: function (data) {
				if (thesaurus=='all')
					return '<p>'+data.prefLabel+'&nbsp;<font color=grey><i>('+data.vocab+')</i></font></p>';
				else
					return '<p>'+data.prefLabel+'</p>';
			}
		},
		display: function (data) {
			return data.prefLabel;
		},
		value: function(data) {
			return data.prefLabel;
		}
	});
}