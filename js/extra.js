var extra_more_collapse = true;

$('.extra-show-js').click(function(){
	let span=$(this).next('.extra-more');
	if (extra_more_collapse) {
		$('.extra-more').removeClass('extra-show-more');
		$('.extra-more').addClass('extra-hide-more');
		$('.extra-js').removeClass('extra-hide-js');
		$('.extra-js').addClass('extra-show-js');
		$(this).removeClass('extra-show-js');
		$(this).addClass('extra-hide-js'); 
	} else {
		$(this).remove(); // remove more button
	}
	$(span).removeClass('extra-hide-more');
	$(span).addClass('extra-show-more');
});

