
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Replace all '<ws>' by your web service identifier and complete the script where there are 3 dots (...)
//    <ws> must correspond to the ws parameter specified in the 'features' column of the 'config_term.txt' configuration file.
//
// Web service managed using Twitter Typeahead
//    See agroportal_typeahead.js, bioportal_typeahead.js, ols_typeahead.js, VO_typeahead.js as examples
//    See https://github.com/twitter/typeahead.js/blob/master/doc/jquery_typeahead.md
//
//    See ROR API example
//        API Doc : https://ror.readme.io/docs/rest-api
//        Demo : https://ror-community.github.io/ror-typeahead-demos/
//        Source code : https://github.com/ror-community/ror-typeahead-demos
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


// Variables used only in this script.
var <ws>_api='https://...'
var <ws>_options='...'
var <ws>_limit='...'

// this function must be named as <ws>_typeahead
// idName : the magggot field name 
// ontology : vocabulary/ontology name(s)
var <ws>_typeahead = function (idName, ontology)
{
	$('#<ws>-'+idName+' .typeahead').typeahead({
		hint: true,
		highlight: true,
		minLength: 3
	},
	{
		limitget: <ws>_limit,
		limitview : <ws>_limit,
		async: true,
		source: function (query, processSync, processAsync) {
			url = <ws>_api + ...+ <ws>_options + ... + '*'+encodeURIComponent(query)+'*'
			if (DEBUG) console.log('GET '+url)
			return $.ajax({
				url: url,
				type: 'GET',
				dataType: 'json',
				success: function (json) {
					...
					// the final returned json must be structured as follow :
					// [ { <label>:"term1", ... },{ <label>:"term2", ... },{ <label>:"term3", ... }, ... ]
					return processAsync(json....);
				}
			});
		},
		templates: {
			header: [ ... ].join('\n'),
			pending: [ ... ].join('\n'),
			empty: [ ... ].join('\n'),
			suggestion: function (data) {
				return data.... ;
			}
		},
		display: function (data) {
			return data....;
		},
		value: function(data) {
			return data....;
		}
	});
}
