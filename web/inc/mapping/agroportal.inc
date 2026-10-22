<?php

//----------------------------
// AgroPortal API
//----------------------------
// Corresponding mapping script when 'agroportal' is specified in the CVtype column of the mapping file (conf/mapping.txt)
//
// Note (oct 2025) : We are using the API with a token because simple searches without a token no longer work correctly 
//                   via curl calls from a server (server side curl blocked by Cloudflare, which uses anti-scraping methods)
//                   For more details, search in Google "server side curl blocked by cloudflare" or "How To Bypass Cloudflare"

function agroportal_search($map, $query)
{
	return ontoportal_mapping($map, $query, 'agroportal');
}
