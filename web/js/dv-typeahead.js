
// Dataverse Search API with help of Typeahead
function dvTypeahead () {
	$('#push .typeahead').typeahead({
		hint: true,
		highlight: true,
		minLength: 3
	},
	{
		limitget: 10,
        limitview : 6,
		async: true,
		source: function (query, processSync, processAsync) {
			url = DV_API_URL + '?type=dataverse&q=' + encodeURIComponent(query)+'*';
			return $.ajax({
				url: url,
				type: 'GET',
				dataType: 'json',
				success: function (json) {
					orgs = json.data.items
					return processAsync(orgs);
				}
			});
		},
		templates: {
			pending: [
				'<div class="empty-message">',
				'<b><i>&nbsp;this dataverse seems not to exist...</i></b>',
				'</div>'
			].join('\n'),
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