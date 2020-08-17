#!/usr/bin/php
<?php
#XTD:xkolec07

/**
 * predmet:  IPP
 * projekt:  PHP XTD
 * @author: David Koleckar, xkolec07
 */

mb_internal_encoding('UTF-8'); //kodovani v utf-8
$args = array(); //globalni promena s argumenty

/**
 * Funkce vytiskne chybu na standartni chybovy vystup 
 * @param text 
 * @param code
 * @return exit code
 */
function printError($text, $code){
  global $args;
  fprintf(STDERR, $text);
  if(file_exists($args["output"]))
    unlink($args["output"]);
  exit($code);
}

/**
 * Vypise napovedu
 * @return 0
 */
function help() {
  echo "PHP XML2DDL\n";
  echo "--help            Vyprintsqlne napovedu\n";
  echo "--input=file      Vstupni soubor \n";
  echo "--output=file     Vystupni soubor \n";
  echo "--header=hlavicka vypise na zacatek souboru hlavicku \n";
  echo "--etc=n           n>=0, pocet sloupcu stejnojmenych elementu \n";
  echo "-a                nebudou se generovat sloupce z atributu \n";
  echo "-b                pokud bude element obsahovat vice podelementu \n";
  echo "                  stejneho nazvu, bude se uvazovat, jako by zde byl jediny\n";
  echo "-g                relation \n";
  exit(0); 
}

/**
 * Funkce na parsovani argumentu 
 * @param argc
 * @param argv
 */
function arguments($argc, $argv) {

  $shortopts = "";
  $shortopts .= "abg"; // -a-b-g
  $longopts = array(
    "help", // --help
    "input:", // --input=
    "output:", // --output=
    "etc:", // --etc=n
    "header:", //--header='hlavicka'
    "isvalid:", //--isvalid=filename
  );
  global $args;

  $args = getopt($shortopts, $longopts);

  if($argc <> sizeof($args)+1)
    printError("Argumenty nebyly spravne zadane\n", 1);

  if(isset($args["help"])) {
    if(isset($args["g"]))//nesmi byt zadano g
      printError("prepinac -g a --help nesmi byt spolu\n", 1);
    if($argc > 2) //help musi byt sam
      printError("prepinac --help musi byt sam\n", 1);
    else //vypiseme napovedu
      help();
  }

  //-b nesmi byt s --etc=n
  if(isset($args["b"]) && isset($args["etc"]))
    printError("etc nemuze byt s b\n", 1);

  //kontrola zda je n u etc >=0
  if(isset($args["etc"]) && $args["etc"]< 0)
      printError("etc musi mit n>=0\n", 100);

}

/**
 * Funkce na rozpoznani datoveho typu 
 * @param string (string)
 * @param atribut (boolean)
 * @return int
 */
function type($string, $atribut){
    $string = trim($string);
    if(empty($string) || mb_strtolower($string) == "true" || mb_strtolower($string) == "false" || $string === "0" || $string === "1") return 1;
    if(preg_match("/^[-+]?[0-9]+$/", $string)) return 2;
    if(preg_match("/^[-+]?[0-9]*\.?[0-9]*([eE][-+]?[0-9]+)?$/",$string)) return 3;
    if($atribut) return 4;
    return 5;
}


/**
 * Funkce k parsovani xml dokumentu 
 * @param a (boolean)
 * @param tables (array)
 * @param atributs (array)
 * @param textval (array)
 * @param aux (xml array)
 * @param parent (string)
 */
function xmltoarray($a, &$tables, &$atributs, &$textval, $aux, $parent) {         
   global $koren; //korenovy element xml
   $pomarr = array(); //pomocne pole
   foreach ($aux as $current => $person) {
      //aktualni element
      $current = mb_strtolower($current); 
      //je to element?
      if ($aux->xpath('.') == array($aux)){
        if(!array_key_exists($current, $tables)){
          $tables[$current] = array();
          $atributs[$current] = array();
          $textval[$current] = array();
        }
        //prepinac -a
        if($a){
          foreach ($person->attributes() as $k=>$v) {
            $pom=mb_strtolower($k);
            $atributs [$current][$pom] = type($v,true);
          }
        }
        //pocet vyskytu podelementu do pomocneho pole
        if(array_key_exists($current, $pomarr)){
          $pomarr[$current]++; //pokud v poli je nastavim hodnota+1
        }
        else {//pokud v poli neni nastavim na 1
          $pomarr[$current] = 1;
        }
        //rekurzivni volani funkce 
        xmltoarray($a, $tables,$atributs,$textval,$person,$current); 
      }
      //zpracovani textoveho obsahu elementu
      //orezu mezery pred a za 
      $content=trim($person);        
      if (strlen($content)>0) {
        //zjistim dat. typ
        $typ = type($content,false);
        if(array_key_exists("value", $textval[$current])){
          if($textval[$current]["value"] < $typ)
            $textval[$current]["value"] = $typ;
          }
          else{
            $textval[$current]["value"] = $typ;
          }         
        }
      //z pomocneho pole si ulozim pocet vyskytu podelementu
      if($parent!=$koren){
        foreach ($pomarr as $key => $value) {
          if(array_key_exists($key, $tables[$parent])) {
            if($tables[$parent][$key] < $pomarr[$key]) {
              $tables[$parent][$key] = $pomarr[$key];
            }
          }
          else {
            $tables[$parent][$key] = $pomarr[$key];
          }
        }
      }
    }//konec prvniho foreach
}//konec fce


/**
 * Funkce pro vypsani vystupu programu a kontrola konfliktu
 * @param tables (array)
 * @param atributs (array)
 * @param textval (array)
 * @return out (string)
 */
function printsql($tables,$atributs,$textval){
  $out = "";
  foreach ($tables as $key => $value) {
    //vytisknu primarni klic
    $out .= sprintf("CREATE TABLE %s(\n\tprk_%s_id INT PRIMARY KEY", $key, $key);
    foreach ($tables[$key] as $i => $v) {
      //kontrola konfliktu
      if("prk_".$key."_id" == $i."_id")
        printError("konflikt1\n",90);
      //vytisknu podelementy
      $out .= sprintf(",\n \t%s_id INT",$i);
      //kontrola konfliktu
      foreach ($atributs[$key] as $o => $r){
        if($i."_id" == $o)
          printError("konflikt2\n",90);
      }
    }
    //vytisknu atributy
    foreach ($atributs[$key] as $x => $p){
      //kontrola konfliktu
      if("prk_".$key."_id" == $x)
        printError("konflikt3\n",90);
      switch($p){//zjistim datovy typ
        case 1:
          $type="BIT";
          break;
        case 2:
          $type="INT";
          break;
        case 3:
          $type="FLOAT";
          break;
        case 4:
          $type="NVARCHAR";
          break;
        case 5:
          $type="NTEXT";
          break;
      }
      $out .= sprintf(",\n \t%s %s", $x,$type);
    }
    //vytisknu textovy obsah
    foreach ($textval[$key] as $x => $p) {
      switch($p){//zjistim datovy typ
        case 1:
          $type="BIT";
          break;
        case 2:
          $type="INT";
          break;
        case 3:
          $type="FLOAT";
          break;
        case 4:
          $type="NVARCHAR";
          break;
        case 5:
          $type="NTEXT";
          break;
      }
      $out .= sprintf(",\n \t%s %s", $x,$type);
    }
    //uzavru CREATE TABLE 
    $out .= sprintf("\n);\n\n");
    }
  //vratim textovy obsah
  return $out;
}

/**
 * Pokud bude element obsahovat vice podelementu stejneho nazvu 
 * bude se uvazovat jako by zde byl pouze jediny takovy 
 * @param tables (array)
 */
function prepinacb(&$tables){
  foreach ($tables as $key => $value) {
    foreach ($tables[$key] as $i => $v) {
      if($tables[$key][$i] > 1){//vice podelementu stejneho nazvu      
        for($c=1;$c<($v+1);$c++){
          $tables[$key][$i.$c]=1;
        }
        unset($tables[$key][$i]);//unset puvodni
      }
    }
  }
}

/**
 * Pro n>=0 urcuje maximalni pocet sloupcu vzniklych
 * ze stejnojmenych podelementu 
 * @param tables (array)
 * @param num (int)
 */
function etc(&$tables,$num){
  foreach ($tables as $key => $value) {
    foreach ($tables[$key] as $i => $v) {
       if($v > $num){
        $tables[$i][$key] = 0;//prohodim
        unset($tables[$key][$i]);//unset puvodni
       }
    }
  }
}

/**
 * Funkce zaznamenani vztahu do pole gtab 
 * @param tables (array)
 * @param gtab (array)
 * @return out (string)
 */
function prepinacg($tables,&$gtab){
  foreach ($tables as $a => $value) {
    if(!array_key_exists($a, $gtab))
      $gtab[$a] = array();
    foreach ($tables[$a] as $b => $v) {
      if($a != $b){
        if(array_key_exists($b, $gtab)){
          if(!array_key_exists($b, $gtab[$a])){
            $gtab[$a][$b]= "N:1";            
          }
          if(!array_key_exists($a, $gtab[$b])){
            $gtab[$b][$a]= "1:N";
          }
          else{
            if(array_key_exists($a, $gtab[$b]))
              $gtab[$a][$b]= "N:M";
            if(array_key_exists($b, $gtab[$a]))
              $gtab[$b][$a]= "N:M";
          }
        }
        else{
          if(!array_key_exists($b, $gtab[$a]))
            $gtab[$a][$b]= "N:1";
          if(!array_key_exists($a, $gtab[$b]))
            $gtab[$b][$a]= "1:N";
        }
      }
      if($a == $b){
         $gtab[$a][$b] == "1:1";
      }
    }//konec foreach$tables[$a]
  }//konec prvniho foreach

  do{
    $cyklus=false;
    foreach ($gtab as $a => $v1) {
     foreach ($gtab[$a] as $b => $v2) {
        foreach ($gtab[$b] as $c => $v3){
            if(($c != $a) && !array_key_exists($c, $gtab[$a])){
              $cyklus=true;
              if($gtab[$a][$b] == $gtab[$b][$c])
                $gtab[$a][$c] = $gtab[$a][$b];
              else
                $gtab[$a][$c] = "N:M";
            }
        }
     }
   }
  }while($cyklus);

  //print 
  $out = "";
  $out .= sprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  $out .= sprintf("<tables>\n");
  foreach ($gtab as $mem => $v1) {
    $out .= sprintf("\t<table name=\"%s\">\n",$mem);
    $out .= sprintf("\t\t<relation to=\"%s\" relation_type=\"1:1\" />\n",$mem);
    foreach ($gtab[$mem] as $m => $v2) {
      $out .= sprintf("\t\t<relation to=\"%s\" relation_type=\"%s\" />\n",$m,$gtab[$mem][$m]);
    }
    $out .= sprintf("\t</table>\n");
  }
  $out .= sprintf("</tables>");
  return $out;
}

/**
 * Funkce pro kontrolu validity
 * @param tables (array)
 * @param atributs (array)
 * @param textval (array)
 * @param tables_valid (array)
 * @param atributs_valid (array)
 * @param textval_valid (array)
 * @return boolean
 */
function validace($tables, $atributs, $textval, $tables_valid, $atributs_valid,$textval_valid){
  foreach ($tables_valid as $a => $v1) {
    if(!array_key_exists($a, $tables))
      return false;
    foreach ($tables_valid[$a] as $b => $v2){
    if(!array_key_exists($b, $tables[$a]))
      return false;
    }
  }
  foreach ($atributs_valid as $a => $v1) {
    if(!array_key_exists($a, $atributs))
      return false;
    foreach ($atributs_valid[$a] as $b => $v2){
    if(!array_key_exists($b, $atributs[$a]))
      return false;
    }
  }

  foreach ($textval_valid as $a => $v1) {
    if(!array_key_exists($a, $textval))
      return false;
    foreach ($textval_valid[$a] as $b => $v2){
    if(!array_key_exists($b, $textval[$a]))
      return false;
    }
  }
  return true;
}

//MAIN------------------------------
//zpracuju argumenty
arguments($argc, $argv);

//--input
if(isset($args["input"])){ 
  if(!file_exists($args["input"]))
    printError("file_exists\n", 2);
  if(!is_readable($args["input"]))
    printError("is_readable\n", 2);
  if(($input = fopen($args["input"], "r")) === false) 
      printError("input file\n", 2);
  //nactu soubor xml
  $aux = simplexml_load_file($args["input"]);
} else{
  $input = fopen("php://stdin", "r");
  //musim nacist text xml ze stdin
  $xmlstring="";
  while (false !== ($ch = fgetc($input))) {
    $xmlstring .= $ch;
  }
  $aux = simplexml_load_string($xmlstring);
}

//--output
if(isset($args["output"])) {
  if(empty($args["output"]))
    printError("nezadany soubor\n", 3);
  if(($output = fopen($args["output"], "w+")) === false) {
    printError("output file", 3);
  }
} else{
  $output = fopen("php://stdout", "w");
}

//prepinac -a
$a=true;
if(isset($args["a"])){
  $a=false; //nebudou se generovat sloupce z atributu
}

$tables = array();
$atributs = array();
$textval = array();
xmltoarray($a, $tables, $atributs,$textval, $aux, $koren);
#pro validaci
$tables_valid = array();
$atributs_valid  = array();
$textval_valid  = array();
if(isset($args["isvalid"])){
  //otevreni souboru
  if(!file_exists($args["isvalid"]))
    printError("file_exists\n", 2);
  if(!is_readable($args["isvalid"]))
    printError("is_readable\n", 2);
  if(($input = fopen($args["isvalid"], "r")) === false) 
      printError("input file\n", 2);
  //nactu soubor xml
  $aux = simplexml_load_file($args["isvalid"]);
  $koren =  $aux->getName();
  //naplnim tabulky valid
  xmltoarray($a, $tables_valid, $atributs_valid,$textval_valid, $aux, $koren);
  //prepinac -b
  if(!isset($args["b"]))
    prepinacb($tables_valid);
  fclose($input);
}

//prepinac --etc
if(isset($args["etc"]))
  etc($tables,$args["etc"]);

//prepinac -g
$gtab = array();
if(isset($args["g"])){
  $out="";
  $out .= prepinacg($tables,$gtab);
  fprintf($output, "%s" , $out);
  fclose($output);
  fclose($input);
  exit(0);
}

//prepinac -b
if(!isset($args["b"]))
  prepinacb($tables);

//hlavicka
$out="";
if(isset($args["header"]))
  $out .= sprintf("--%s\n\n",$args["header"]);

//tisk do vystupniho souboru a kontrola validity
if(isset($args["isvalid"]))
  if(validace($tables, $atributs, $textval, $tables_valid, $atributs_valid,$textval_valid))
    $out .= printsql($tables,$atributs,$textval);
  else
    printError("Nevalidni",91);
else
  $out .= printsql($tables,$atributs,$textval);

fprintf($output, "%s" , $out);
fclose($output);
fclose($input);
?>