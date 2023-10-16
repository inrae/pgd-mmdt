function zcTypeahead () {
	$('#push .typeahead').typeahead({
		hint: true,
		highlight: true,
		minLength: 3
	},
	{
		async: true,
		source: function (query, processSync, processAsync) {
			url = ZC_API_URL + encodeURIComponent(query);
			return $.ajax({
				url: url,
				type: 'GET',
				dataType: 'json',
				success: function (json) {
					return processAsync([json]);
				}
			});
		},
		templates: {
			pending: [
				'<div class="empty-message">',
				'<b><i>&nbsp;this community seems not to exist...</i></b>',
				'</div>'
			].join('\n'),
			suggestion: function (data) {
				if(data.metadata.title.length > 0)
					return '<p>'+data.metadata.title+'</p>';
			}
		},
		display: function (data) {
			return data.slug;
		},
		value: function(data) {
			return data.slug;
		}
	});
}