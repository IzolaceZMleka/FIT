<?php

#DKA:xpavli78
/*
 * Soubor: args.php
 * Datum: 9.3.2016
 * Autor: Jan Pavlica, xpavli78@stud.fit.vutbr.cz
 * Projekt: Determinizace konecneho automatu (DKA)
 * Predmet: IPP
 * Popis: zpracovani argumentu
*/

function get_params(&$argc, &$argv, &$eps, &$det, &$sens, &$in_file, &$out_file)
{
	$in = false; // je vstupni soubor
	$out = false; // je vystupni soubor
	
	if($argc==2 && $argv[1] == "--help") // napoveda
	{
		echo
		"_____________________________________________Napoveda________________________________________\n".
		"Skript pro zpracovani a determinizaci konecneho automatu\n\n".
		"Pouziti:\n\n".
		"--help \t vypise napovedu\n".
		"--input=filename\t cesta ke vstupnimu souboru s konecnym automatem\n".
		"--output=filename\t cesta k vystupnimu souboru\n" .
		"-e, --no-epsilon-rules\t odstraneni epsilon pravidel\n".
		"-d, --determinization\t determinizace konecneho automatu\n".
		"-i, --case-insensitive\t nebude bran ohled na velikost znaku pri porovnavani stavu ci symbolu\n";
		
		exit(0);
	}
	else
	{
		foreach($argv as $value) // kontroluji jednotlive argumenty
		{
			switch($value)
			{
				
				case "-e":
					if(!$eps && !$det)
					{
						$eps = true;
						break;
					}
					else
					{
						exit(1);
					}
				case "--no-epsilon-rules":
					if(!$eps && !$det)
					{
						$eps = true;
						break;
					}
					else
					{
						exit(1);
					}
				case "--case-insensitive":
					if(!$sens)
					{
						$sens = true;
						break;
					}
					else
					{
						exit(1);
					}
				case "-d":
					if(!$eps && !$det)
					{
						$det = true;
						break;
					}
					else
					{
						exit(1);
					}
				case "-i":
					if(!$sens)
					{
						$sens = true;
						break;
					}
					else
					{
						exit(1);
					}
				case "--determinization":
					if(!$eps && !$det)
					{
						$det = true;
						break;
					}
					else
					{
						exit(1);	
					}
				case "--help":
						exit(1);
				default:
					if(!strncmp("$value","--input=",8))
					{
						if($in)
						{
							exit(1);
						}
						else
						{
							$in = true;
							$in_file = substr("$value",8); // cesta k souboru
							break;
						}
					}
					elseif(!strncmp("$value","--output=",9))
					{
						if($out)
						{
							exit(1);
						}
						else
						{
							$out = true;
							$out_file = substr("$value",9); // cesta k souboru
							
							if(is_file($out_file))
								break;
								
							exit(3);
						}
					}
					else{
						exit(1);
						break;
					}
			}
		}
	}

	if(empty($in_file))
	{
		$in_file = "php://stdin";
	}
	if(empty($out_file))
	{
		$out_file = "php://stdout";
	}

	return;
}


?>
