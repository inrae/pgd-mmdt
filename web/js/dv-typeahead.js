// Dataverse Search API with help of Typeahead
var dvError = ''; // Track error state

function dvTypeahead () {
	var $input = $('#push .typeahead');
	
	$input.typeahead({
		hint: true,
		highlight: true,
		minLength: 3
	},
	{
		limitget: 50,
        limitview : 50,
		async: true,
		source: function (query, processSync, processAsync) {
			// Debug: log the token value
			if (DEBUG) {
				console.log('DV_API_TOKEN value: "' + DV_API_TOKEN + '"');
				console.log('Token type: ' + typeof DV_API_TOKEN);
				console.log('Token is empty: ' + (!DV_API_TOKEN || (typeof DV_API_TOKEN === 'string' && DV_API_TOKEN.trim() === '')));
			}
			
			// Check if API token is available before making request
			if (!DV_API_TOKEN || (typeof DV_API_TOKEN === 'string' && DV_API_TOKEN.trim() === '')) {
				if (DEBUG) console.log('Dataverse API token not provided - will show empty message');
				dvError = 'noToken';  // Set error state
				// Show the "empty" template with error message immediately
				processSync([]);
				// Return empty array immediately - don't make any AJAX call
				processAsync([]);
				return;  // Important: return nothing, don't return processAsync
			}
			dvError = '';  // Clear error when token is provided
			url = DV_API_URL + '?type=dataverse&q=' + encodeURIComponent(query)+'*';
			if (DEBUG) console.log('GET '+url)
			return $.ajax({
				url: url,
				type: 'GET',
				dataType: 'json',
				timeout: 5000,
				headers: {
					'X-Dataverse-key': DV_API_TOKEN
				},
				success: function (json) {
					orgs = json.data.items
					return processAsync(orgs);
				},
				error: function (xhr) {
					if (DEBUG) console.log('Error: ' + xhr.status);
					if (xhr.status === 401) {
						dvError = 'invalidToken';
					} else {
						dvError = 'apiError';
					}
					return processAsync([]);
				}
			});
		},
		templates: {
			pending: [
				'<div class="empty-message">',
				'<img src="https://guides.dataverse.org/en/latest/_images/dataverse-icon.jpg" style="width:30px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp; ... wait ...</i></b>',
				'</div>'
			].join('\n'),
			empty: function() {
				var msg = 'This dataverse seems not to exist...';
				if (dvError === 'noToken') {
					msg = 'Please provide your API Token above to search';
				} else if (dvError === 'invalidToken') {
					msg = 'Invalid or expired API Token. Please check your credentials.';
				} else if (dvError === 'apiError') {
					msg = 'Error connecting to Dataverse. Please try again.';
				}
				return '<div class="empty-message" style="padding: 10px; white-space: nowrap;">' +
					'<img src="https://guides.dataverse.org/en/latest/_images/dataverse-icon.jpg" style="width:20px; vertical-align: middle;"/>&nbsp;' +
					'<b style="font-size: 14px;"><i>' + msg + '</i></b>' +
					'</div>';
			},
			suggestion: function (data) {
				return '<p>'+data.name+'</p>';
			}
		},
		display: function (data) {
			return data.identifier;
		},
		value: function(data) {
			return data.identifier;
		}
	});
}
