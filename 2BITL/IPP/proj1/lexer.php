<?php

#DKA:xpavli78
/*
 * Soubor: lexer.php
 * Datum: 9.3.2016
 * Autor: Jan Pavlica, xpavli78@stud.fit.vutbr.cz
 * Projekt: Determinizace konecneho automatu (DKA)
 * Predmet: IPP
 * Popis: zpracovani vstupu, syntakticka a semanticka kontrola
*/

class rule
{
	public $from;
	public $symbol;
	public $to;
}

// funkce pro ziskani znaku

function get_char($sens, &$file_content)
{
	$c = mb_substr($file_content,0,1);
	$file_content = mb_substr($file_content,1);
	
	return $c;
}

// funkce pro pridani stavu a kontrolu duplikaci

function add_state(&$states, $l_state)
{
	foreach($states as $compared_state)
	{
		if($compared_state == $l_state)
			return;
	}
	
	array_push($states, $l_state);
}

// funkce pro pridani znaku abecedy a kontrolu duplikaci

function add_alphabet(&$alphabet, $symbol)
{
	foreach($alphabet as $compared_alphabet)
	{
		if($compared_alphabet == $symbol)
			return;
	}
	array_push($alphabet, $symbol);
}

// funkce pro pridani pravidla a kontrolu duplikaci

function add_rule(&$rules, $rule)
{
	foreach($rules as $compared_rule)
	{
		if($compared_rule == $rule)
			return;
	}
	array_push($rules, $rule);
}

// kontrola zda je stav (konecny/pocatecni) v mnozine stavu

function check_state($states, $p_state)
{
	$ok = false;
	
	foreach($states as $state)
	{
		if($p_state==$state)
		{
			$ok = true;
		}
	}
	
	if($ok)
	{
		return $p_state;
	}
	else
	{
		exit(41);
	}
}

// kontrola zda je cteny symbol v abecede

function check_symbol($alphabet, $p_symbol)
{
	$ok = false;
	
	foreach($alphabet as $alpha)
	{
		if($p_symbol==$alpha)
		{
			$ok = true;
		}
	}
	
	if($ok)
	{
		return $p_symbol;
	}
	else
	{
		exit(41);
	}
}


// fuknce pro ziskani automatu

function get_fsm(&$file_content, &$alphabet, &$states, &$i_state, &$rules, &$f_states, $sens)
{
	$a_state = 0;
	$p_state;
	$c;
	$loaded = 0;
	$rule;
	
	while(mb_strlen($file_content)) // dokud jsou v souboru znaky
	{
		$c = get_char($sens,$file_content);
		
// 		echo $c;
// 		echo $a_state;
		
		if($a_state != 7 && $a_state != 12 && $a_state != 13 &&ctype_space($c)) // preskakuji bile znaky
		{
			continue;
		}
		elseif($a_state != 7 && $a_state != 12 && $c == "#") // preskakuji komentare
		{
			while(mb_strlen($file_content))
			{
				$c = get_char($sens,$file_content);
				if($c == "\n")
					break;
			}
			continue;
		}
		
		switch($a_state)
		{
			case 0: // pocatecni stav
				if($c == "(")
				{
					$a_state = 1;
					break;
				}
				else
					exit(40);
			case 1: // zacatek mnoziny stavu
				if($c == "{")
				{
					$a_state = 2;
					break;
				}
				else
					exit(40);
			case 2: // prvni znak stavu
				if((ctype_alpha($c)) && ($c != '_'))
				{
					$tmp_state = $c;
					$a_state = 3;
					break;
				}
				else
					exit(40);
			case 3: // docteni zbytku stavu a ulozeni
				if($c == ',' || $c == '}')
				{
					if(mb_substr($tmp_state, -1) == "_") // kontrola na podtrzitka
						exit(40);
					else
					{
						add_state($states,$tmp_state);
						$a_state = 2;
						if($c == '}')
							$a_state = 4;
						break;
					}
				}
				elseif(ctype_alnum($c) || $c == "_")
				{
					$tmp_state .= $c;
					break;
				}
				else
					exit(40);
			case 4: // oddelovac mezi stavy a abecedou
				if($c == ",")
				{
					$a_state = 5;
					break;
				}
				else
					exit(40);
			case 5: // zacatek abecedy
				if($c == "{")
				{
					$a_state = 6;
					break;
				}
				else
					exit(40);
			case 6: // zacatek prvniho symbolu
				if($c == "'")
				{
					$a_state = 7;
					break;
				}
				elseif($c == "}")
				{
					$a_state = 8;
					exit(41);
				}
				else
					exit(40);
			case 7: // nacitani symbolu
				if($c == "'") // pokud je ''
				{
					$c = get_char($sens,$file_content);
					
					if($c == "'") // pri '''
					{
						
						$c = get_char($sens,$file_content);
						if($c == "'") // pri ''''
						{
							
							$c = get_char($sens,$file_content);
							
							while(ctype_space($c))
								$c = get_char($sens,$file_content);
					
							if($c == ',' || $c == "}")
							{
								add_alphabet($alphabet, "'");
								
								if($c == "}")
								{
									$a_state = 8;
									break;
								}
								$a_state = 6;
								break;
							}
							else
								exit(40);
						}
						else
							exit(40);
					}
					
					while(ctype_space($c))
						$c = get_char($sens,$file_content);
					
					if($c == ',' || $c == "}") // vpripade, ze narazim na , nebo }
					{
						add_alphabet($alphabet, "eps"); // pridam epsilon prechod
						if($c == "}") // skacu na konec abecedy
						{
							$a_state = 8;
							break;
						}
						$a_state = 6;
					}
					else
						exit(40);
				}
				else
				{
					$t_c = get_char($sens,$file_content);
					
					if($t_c == "'")
					{
							add_alphabet($alphabet, $c);
							
							$t_c = get_char($sens,$file_content);
							
							while(ctype_space($t_c))
								$t_c = get_char($sens,$file_content);
							
							if($t_c == ',' || $t_c == "}")
							{
								if($t_c == "}")
								{
									$a_state = 8;
									break;
								}
								$a_state = 6;
								break;
							}
					}
					else
						exit(40);
					break;
				}
				case 8: // mezi symbolem a pravidly
					if($c == ",")
					{
						$a_state = 9;
						break;
					}
					else
						exit(40);
				case 9: // zacatek pravidel
					if($c == "{")
					{
						$a_state = 10;
						break;
					}
					else
						exit(40);
				case 10: // prvni znak stavu
					if($c === "}")
					{
						$a_state = 16;
						break;
					}
					$rule = new rule();
					if((ctype_alpha($c)) && ($c != '_'))
					{
						$tmp_state = $c;
						$a_state = 11;
						break;
					}
					else
						exit(40);
				case 11: // nacteni zbytku stavu
					if($c == "'")
					{
						if(mb_substr($tmp_state, -1) == "_")
							exit(40);
						else
						{
							$rule->from = check_state($states,$tmp_state);
							$a_state = 12;
							break;
						}
					}
					elseif(ctype_alnum($c) || $c == '_')
					{
						$tmp_state .= $c;
						break;
					}
					else
						exit(40);
				case 12: // nacitani symbolu
					if($c == "'")
					{
						$c = get_char($sens,$file_content);
						
						while(ctype_space($c))
							$c = get_char($sens,$file_content);
						
						if($c == '-')
						{
							$rule->symbol = "eps";
							$a_state = 13;
							break;
						}
						elseif($c == "'")
						{
							$c = get_char($sens,$file_content);
							if($c == "'")
							{
								$c = get_char($sens,$file_content);
								
								while(ctype_space($c))
									$c = get_char($sens,$file_content);
						
								if($c == '-')
								{
									$rule->symbol = check_symbol($alphabet, "'", $rule);
									$a_state = 13;
									break;
								}
								else
									exit(40);
							}
							else
								exit(40);
						}
						break;
					}
					else
					{
						$t_c = get_char($sens,$file_content);
						
						if($t_c == "'")
						{
								$rule->symbol = check_symbol($alphabet, $c, $rule);
								
								$t_c = get_char($sens,$file_content);
								
								while(ctype_space($t_c))
									$t_c = get_char($sens,$file_content);
								
								if($t_c == '-')
								{
									$a_state = 13;
									break;
								}
								else
									exit(40);
						}
						else
							exit(40);
						break;
					}
				case 13: // mezi symbolem a stavem
					if($c == ">")
						{
							$a_state = 14;
							break;
						}
					else
						exit(40);
				case 14: // prvni znak stavu
					if((ctype_alpha($c)) && ($c != '_'))
					{
						$tmp_state = $c;
						$a_state = 15;
						break;
					}
					else
						exit(40);
				case 15: // docteni zbytku stavu a ukonceni
					if($c == ',' || $c == '}')
					{
						if(mb_substr($tmp_state, -1) == "_")
							exit(40);
						else
						{
							$rule->to = check_state($states,$tmp_state);
							add_rule($rules,$rule);
							$a_state = 10;
							if($c == '}')
							{
								$a_state = 16;
							}
							break;
						}
					}
					elseif(ctype_graph($c))
					{
						$tmp_state .= $c;
						break;
					}
					else
						exit(40);
				case 16: // mezi pravidly a pocatecni stavem
					if($c == ",")
					{
						$a_state = 17;
						break;
					}
					else
						exit(40);
				case 17: // první znak pocatecniho stavu
					if((ctype_alpha($c)) && ($c != '_'))
					{
						$tmp_state = $c;
						$a_state = 18;
						break;
					}
					else
						exit(40);
				case 18: // doceteni zbytku stavu a ukonceni
					if($c == ',')
					{
						if(mb_substr($tmp_state, -1) == "_")
							exit(40);
						else
						{
							$i_state = check_state($states,$tmp_state);
							$a_state = 19;
							break;
						}
					}
					elseif(ctype_alnum($c) || $c == "_")
					{
						$tmp_state .= $c;
						break;
					}
					else
						exit(40);
				case 19: // zacatek koncovych stavu
					if($c == "{")
					{
						$a_state = 20;
						break;
					}
					else
						exit(40);
				case 20: // první znak stavu
					if($c == "}")
					{
						$a_state = 22;
						break;
					}
					elseif((ctype_alpha($c)) && ($c != '_'))
					{
						$tmp_state = $c;
						$a_state = 21;
						break;
					}
					else
						exit(40);
				case 21: // docteni zbytku stavu a ukonceni
					if($c == ',' || $c == '}')
					{
						if(mb_substr($tmp_state, -1) == "_")
							exit(40);
						else
						{
							add_state($f_states,check_state($states,$tmp_state));
							$a_state = 20;
							if($c == '}')
								$a_state = 22;
							break;
						}
					}
					elseif(ctype_alnum($c) || $c == "_")
					{
						$tmp_state .= $c;
						break;
					}
					else
						exit(40);
				case 22: // ukonceni automatu
					if($c == ")")
					{
						$a_state = 23;
						break;
					}
					else
						exit(40);
				case 23: // bile znaky nebo komentare jinak error
					exit(40);
		}
	}
		
	
	
}

?>
