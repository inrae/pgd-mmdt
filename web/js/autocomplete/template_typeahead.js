
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

// Mandatory variable : <ws>_ontology 
// it will be initialized with the 'onto' parameter specified in the 'features' column of the 'config_term.txt' configuration file.
var <ws>_ontology=''

// other variables used only in this script.
var api='https://...'
var options='...'


// this function must be named as <ws>_typeahead
function <ws>_typeahead () {
	$('#<ws> .typeahead').typeahead({
		hint: true,
		highlight: true,
		minLength: 3
	},
	{
		limitget: 50,
		limitview : 30,
		async: true,
		source: function (query, processSync, processAsync) {
			url = api + ...
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
