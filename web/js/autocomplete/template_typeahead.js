
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Replace all '<ws>' by your web service identifier and complete the script where there are 3 dots (...)
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Mandatory variable : <ws>_ontology 
// it will be initialized with the 'onto' parameter specified in the 'festures' column of the 'config_term.txt' configuration file.
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
