
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Replace all '<ws>' by your web service identifier and complete the script where there are 3 dots (...)
//    <ws> must correspond to the ws parameter specified in the 'features' column of the 'config_term.txt' configuration file.
//
// Web service managed using Twitter Typeahead
//    See https://github.com/twitter/typeahead.js/blob/master/doc/jquery_typeahead.md
//    See ROR API example
//        API Doc : https://ror.readme.io/docs/rest-api
//        Demo : https://ror-community.github.io/ror-typeahead-demos/
//        Source code : https://github.com/ror-community/ror-typeahead-demos
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


// Variables used only in this script.
var <ws>_api='https://...'
var <ws>_options='...'

// this function must be named as <ws>_typeahead
// idName : the magggot field name 
// ontology : vocabulary/ontology name(s)
var <ws>_typeahead = function (idName, ontology) {
	$('#<ws>-'+idName+' .typeahead').typeahead({
		hint: true,
		highlight: true,
		minLength: 3
	},
	{
		limitget: 50,
		limitview : 30,
		async: true,
		source: function (query, processSync, processAsync) {
			url = <ws>_api + ...+ <ws>_options + ... + '*'+encodeURIComponent(query)+'*'
			if (DEBUG) console.log('GET '+url)
			return $.ajax({
				url: url, type: 'GET', dataType: 'json',
				success: function (json) {
					return processAsync(json....);
				}
			});
		},
		templates: {
			header: [ ... ].join('\n'),
			pending: [ ... ].join('\n'),
			empty: [ ... ].join('\n'),
			suggestion: function (data) {
				return data... ;
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
