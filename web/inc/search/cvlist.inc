<?php

// If the field value belongs to one of the user-defined lists, then returns a modified
// (more relevant value) or increased (URL link) value; Otherwise returns the value unchanged.
function metadata_render ($val)
{
	global $cvdir, $vocab_cache_file;

	$ffs = scandir($cvdir);
	unset($ffs[array_search('.', $ffs, true)]);
	unset($ffs[array_search('..', $ffs, true)]);

	$return_val = $val;
	if (count($ffs)>0 ) {
		foreach($ffs as $diconame) {
			$dicofile = $cvdir.$diconame.'/'.$diconame.'.txt';
			if( ! is_dir($cvdir.$diconame) || ! file_exists($dicofile) ) continue;
			$dicovalues = tsvToArray($dicofile);
			foreach ($dicovalues as $values) {
				if ($diconame !== 'people' && $val === $values[0]) {
					for($i=1; $i<count($values); $i++)
						if (preg_match('"^https?://"', $values[$i])) {
							$return_val = '<a href="'.$values[$i].'" target="_blank">'.$val.'</a>';
							break;
						}
				} elseif ($diconame === 'people') {
					$fullname = $values[0].' '.$values[1];
					if ($val === $fullname && strlen($values[3])>0) {
						$return_val = '<a href="https://orcid.org/'.$values[3].'" target="_blank">'.$val.'</a>';
						break;
					}
				}
				if ($return_val != $val) break;
			}
			if ($return_val != $val) break;
		}
	}
	if ($return_val === $val && file_exists($vocab_cache_file)) {
		$out=find_into_cache($val);
		if (count($out)>0)
			$return_val = '<a href="'.$out[1].'" target="_blank">'.$val.'</a>';
	}
	return $return_val;
}

?>