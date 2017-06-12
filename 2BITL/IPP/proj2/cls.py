#!/usr/bin/env python
# -*- coding: utf-8 -*-
#CLS:xpavli78

import sys
import getopt
import re
from lxml import etree

class params: # trida pro uchovani parametru
    input_file = None # vstupni soubor
    output_file = None # vystupni soubor
    k = 4 # pocet mezer pro odsazeni
    detailed_class = None # trida, ktera my byt detailne vypsana
    detailed = False # kontrola prepinace details
    search_xpath = None # xpath vyraz
    conflicts = None # kontrola prepinace conflicts

class classes: # trida pro ulozeni udaju o tride
    
    def __init__(self, inheritance_from = None, methods = None, atributes = None, static = None, own_methods = None, own_atributes = None, childs = None, conflicts = None):
        if inheritance_from is None:
            self.inheritance_from = []
        else:
            self.inheritance_from = inheritance_from
        if methods is None:
            self.methods = []
        else:
            self.methods = methods
        if atributes is None:
            self.atributes = []
        else:
            self.atributes = atributes
        if own_atributes is None:
            self.own_atributes = []
        else:
            self.own_atributes = own_atributes
        if own_methods is None:
            self.own_methods = []
        else:
            self.own_methods = own_methods
        if childs is None:
            self.childs = []
        else:
            self.childs = childs
        if conflicts is None:
            self.conflicts = {}
        else:
            self.conflicts = conflicts
    
    has_pure = "concrete" # pokud obsahuje pure metodu
    
    inheritance_from = [] # (predek - pristup)
    childs = [] # tridy do kterych dedim
    
    methods = [] # seznam vsech metod (typ - jmeno - [atributy] - pristup  - typ_virtual - static - jmeno puvodni tridy - pristup v puvodni tride)
    atributes = [] # seznam vsech atributu (typ - jmeno - pristup - static - jmeno puvodni tridy - pristup v puvodni tride)
    
    own_atributes = [] # seznam vlastnich atributu, stejna struktura jako seznam vsech
    own_methods = [] # seznam vlastnich metod, stejna struktura jako seznam vsech
    
    defined = False # jestli je definovana nebo jenom deklarovana
    
    conflicts = {} # slovnik pro konflikty, klic je jmeno funkce/atributu, hodnota je seznam konfliktnich clenu
    
    
def add_class_into_dic(class_name,list_of_classes): # funkce pro pridani do seznamu trid
    if class_name in list_of_classes and list_of_classes[class_name].defined : # pokud je jiz trida deklarovana
        return
    else: # vlozim do seznamu
        new_class = classes()
        list_of_classes[class_name] = new_class
        
def parse_method(string, my_class, access, regex_data_types, name, virtual_type,regex_data_types_check): # zpracovani udaju o metode
    
    is_static = "instance" # promenna pro ulozeni zda-li je staticka
    
    if re.search('static',string): # kontrola, zda je staticka
        is_static = "static"
    
    method_type = re.search(regex_data_types,string).group(0) # zjisteni datoveho typu metody
    method_type_f = ""
    for i in method_type.split():
        method_type_f += i + ' '
        
    method_type_f = method_type_f[:-1]
    
    
    method_name_splitted = re.split('\s|&|\*',string[:(string.find('('))]) # zjisteni jmena metody
    method_name = method_name_splitted.pop()
    
    while method_name == '' : # preskoceni bilych znaku
        method_name = method_name_splitted.pop()
            
    daccess = "" # odliseni pro konstruktor a destruktor
    if method_name == name : # kontrola zda se nejedna o konstruktor
        method_type_f = method_name
        daccess = "constructor"+access
    elif method_name == ('~'+name): # kontrola zda se nejedna o destruktor
        method_type_f = "void"
        daccess = "destructor"+access
        for my_method in my_class.own_methods: # kontrola pretezovani destruktoru
            if my_method[1] ==  method_name:
                sys.exit(4)
        
    arguments = re.search('\((.|\n)*\)',string).group(0)[1:-1].split(',') # seznam pro argumenty metody
    list_of_arguments = []

    for i in arguments: # pokud metoda nema argumenty, nastavime None
        if i == "void" or i == '' or i == " void" or i == " void ":
            list_of_arguments = None
            break;
        
        list_of_arguments.append(parse_atribut(i, my_class, "for funciton",regex_data_types,name,regex_data_types_check)) # zpracovani argumentu
        
    if virtual_type != (): # zaznam pokud se jedna o pure metodu
        if virtual_type ==  "yes":
            my_class.has_pure = "abstract"
    
    # pridani metody do seznamu vlastnich metod
    
    if daccess != "" :
        my_class.own_methods.append((method_type_f,method_name,list_of_arguments,daccess,virtual_type,is_static,name,access))
    elif access == "public" :
        my_class.own_methods.append((method_type_f,method_name,list_of_arguments,"public",virtual_type,is_static,name,access))
    elif access == "private" :
        my_class.own_methods.append((method_type_f,method_name,list_of_arguments,"private",virtual_type,is_static,name,access))
    elif access == "protected" :
        my_class.own_methods.append((method_type_f,method_name,list_of_arguments,"protected",virtual_type,is_static,name,access))
    
        

def parse_atribut(string, my_class, access, regex_data_types, name,regex_data_types_check): # zpracovani atributu/argumentu
    
    atribut_type = re.search(regex_data_types,string).group(0)
    atribut_type_f = ""
    
    is_static = "instance" # promenna pro ulozeni staticke
    
    if re.search('static',string): # kontrola zda-li je staticka
        is_static = "static"
    
    for i in atribut_type.split(): # zjisteni datoveho typu
        atribut_type_f += i + ' '
        
    atribut_type_f = atribut_type_f[:-1] # vyber datoveho typu
    
    atribut_name = re.split('\s|&|\*|void',string).pop() # zjisteni jmena atributu
    
    if access != "for funciton": # kontrola, zda-li se nejedna o redefinici
        for added_ones in my_class.own_atributes :
            if added_ones[1] == atribut_name:
                sys.exit(4)
        for added_ones in my_class.own_methods :
            if added_ones[1] == atribut_name:
                sys.exit(4)
        
    # pridani do seznamu vlastnich atributu
    if access == "public" :
        my_class.own_atributes.append((atribut_type_f,atribut_name,"public",is_static,name,access))
    elif access == "private" :
        my_class.own_atributes.append((atribut_type_f,atribut_name,"private",is_static,name,access))
    elif access == "protected" :
        my_class.own_atributes.append((atribut_type_f,atribut_name,"protected",is_static,name,access))
        
    if access == "for funciton": # vraceni v pripade, ze se jedna o argument
        return (atribut_type_f,atribut_name)
    
def get_class_name(whole_class): # zjisteni jmena tridy
    whole_class = whole_class.split(':')[0]
    whole_class = whole_class.split(';')[0]
    whole_class = whole_class.split('{')[0]
    whole_class = whole_class.split()
    
    if whole_class[1] == "class": 
        whole_class.pop(0)
    
    return (whole_class[1])

def get_rest(whole_class,list_of_classes,name): # ziskani zbytku informaci o tride
    
    pom_regex_data_types = ['bool','void','char','unsigned char','signed char','int','unsigned int','signed int','short int','unsigned short int','signed short int','long int','signed long int','unsigned long int','float','double','long double','wchar_t'] + (list(list_of_classes.keys())) # regularni vyraz pripustnych datovych typu
    
    regex_data_types = '' # promenna pro ulozeni regularniho vyrazu
    
    body = whole_class[(whole_class.find('{')+1):whole_class.rfind('}')] # ziskani tela tridy
    
    for i in pom_regex_data_types:
        regex_data_types += i + '|'
        
    regex_data_types = regex_data_types[:-1] # tvoreni regularniho vyrazu
    regex_data_types_check = '.*'+regex_data_types+'.*'
    regex_data_types_without_ref = '(' + regex_data_types + ')' + '(\*|&|\s)*\w+'
    regex_data_types_test = '(' + regex_data_types + ')' + '\s*(\*|&|\s)*'
    #regex_data_types_with_ref = '(' + regex_data_types + ')' + '\s*\*\s*\w+'
    
    access = "private" # pokud neni zadano jinak jedna se o privatni pristup
    
    tokens = re.split(';|:|\}', body) # rozdeleni tela do jednotlivych casti

    for record in tokens: # zpracovani jednotlivych casti
        # zjisteni opravneni
        if re.search('\s*public',record) != None:
            access = "public"
        elif re.search('\s*private',record) != None:
            access = "private"
        elif re.search('\s*protected',record) != None:
            access = "protected"
        # zpracovani v pripade uziti klicoveho slova using
        elif re.search('using',record) != None:
            met_at_name = ""
            for j in tokens :
                if re.search('\w+',j) != None:
                    if re.search('using',j) != None:
                        # zjisteni z ktere tridy pouzivam
                        wanna_name = re.split('\s',j)
                        using_from = wanna_name.pop()
                        continue
                    elif re.search('\s*public',j) != None:
                        continue
                    elif re.search('\s*private',j) != None:
                        continue
                    elif re.search('\s*protected',j) != None:
                        continue
                    else :
                        # zjisteni jmena promenne
                        wanna_name = re.split('\s',j)
                        met_at_name = wanna_name.pop(0)
                        break
            
            is_defined = False
            
            # muzu pouzivat pouze z definovanyhc trid
            for key in list_of_classes.keys() :
                if key == using_from :
                    is_defined = True
            
            if is_defined :
                use_name = list_of_classes[using_from]
                
                wasnt_found = True
                # kontrola zda-li je dany atribut v dane tride a zda jej mohu pouzit
                for atribut in use_name.atributes :
                    atributs_name = atribut[1]
                    if met_at_name == atributs_name:
                        if atribut[2] != "dont show" and atribut[2] != "private":
                            list_of_classes[name].own_atributes.append((atribut[0],atribut[1],access,atribut[3],atribut[4]))
                            wasnt_found = False
                        else :
                            sys.exit(4)
                # kontrola zda-li je dana metoda v dane tride a zda ji mohu pouzit
                for method in use_name.methods :
                    methods_name = method[1]
                    if met_at_name == methods_name:
                        if method[3] != "dont show" and method[3] != "private":
                            list_of_classes[name].own_methods.append((method[0],method[1],method[2],access,method[4],method[5],method[6]))
                            wasnt_found = False
                        else:
                            sys.exit(4)
                
                if wasnt_found: # pokud neni pristupna, chyba
                    sys.exit(4)
                
            else : # pokud neni trida definovana chyba
                sys.exit(4)
                
            break
        elif re.search('virtual\s.*static|static\s.*virtual',record) != None:
            sys.exit(4)
        elif re.search('virtual(.|\n)*0',record) != None: # pokud se jedna o pure virtual
            parse_method(record, list_of_classes[name], access, regex_data_types_test, name, "yes",regex_data_types_check)
        elif re.search('virtual(.|\n)*\((.*|\n)*\)',record) != None: # pokud se jedna o virtual
            parse_method(record, list_of_classes[name], access, regex_data_types_test, name, "no",regex_data_types_check)
        elif re.search('=\s*0',record) != None: # pokud se jedna o atribut
            sys.exit(4)
        elif re.search('(.|\n)*{',record) != None: # pokud se jedna o metodu a jeji definici
            parse_method(record, list_of_classes[name], access, regex_data_types_test, name, "normal",regex_data_types_check)
        elif re.search('(.|\n)*\((.*|\n)*\)',record) != None: # pokud se jedna o metodu a jeji definici
            parse_method(record, list_of_classes[name], access, regex_data_types_test, name, "normal",regex_data_types_check)  
        elif re.search(regex_data_types_without_ref,record) != None: # pokud se jedna o atribut
            parse_atribut(record, list_of_classes[name], access, regex_data_types_test, name,regex_data_types_check)
        elif re.search('\w+',record) != None:
            sys.exit(4)
    return
    
def get_classes(my_params): # ziskani vstupnich dat a jejich zpracovani
    
    list_of_classes = {} # slovnik trid, klic - jmeno tridy, hodnota - objekt tridy
    string_from_file = None # string pro data ze vstupniho souboru
    
    if my_params.input_file != None: # otevreni vstupniho souboru
        try:
            my_file = open(my_params.input_file, 'r')
            string_from_file = my_file.read()
            my_file.close()
        except IOError:
            sys.exit(2)
    else : # cteni ze stdin
        string_from_file = sys.stdin.read()
        
    if len(string_from_file) == 0: # v pripade prazdneho souboru
        return None
    
    string_from_file = re.sub('(\s|\n)+',' ',string_from_file) # nahrazeni vetsiho poctu mezer, jednou
    
    all_classes = re.split('[^\w]class[^\w]',string_from_file) # rozdeleni dle jednotlivych trid
    
    for i in all_classes: # zpracovani kazde tridy
        if i != '' :
            whole_class = 'class ' + i # cela definice/deklarace tridy
            name = get_class_name(whole_class) # zjisteni jmena tridy
            
            add_class_into_dic(name,list_of_classes) # pridani do slovniku
            
            my_regex = '\s*class\s+' + name + '\s*(.|\n)*?{' # regex pro definici
            
            if re.search(my_regex,whole_class) != None: # pokud se jedna o definici
                list_of_classes[name].defined = True # zaznam, ze je definovana
                get_rest(whole_class,list_of_classes,name) # ziskani zbyvajicih udaju o tride
                list_of_classes[name].atributes = list(list_of_classes[name].own_atributes) # vlastni atributy se pridaji do seznamu vsech
                list_of_classes[name].methods = list(list_of_classes[name].own_methods) # vlastni metody se pridaji do seznamu vsech
            
                last_control(list_of_classes,name) # kontrola zda nejaka metoda ve tride nesplnuje podminky pretezovani
            
            my_regex = 'class\s+' + name + '\s*:(.|\n)*?{' # regex pro kontrolu dedeni
            
            if re.search(my_regex,whole_class) != None: # pokud trida dedi z jine
                get_inheritance(list_of_classes,whole_class,name,my_params.conflicts) # ziskani dedenych dat
        
        #print_all(name, list_of_classes)
        
    return list_of_classes # vracim seznam zpracovanych trid

def print_all(list_of_classes):
    
    for name in sorted(list_of_classes.keys()):
    
        print("CLASS NAME: "+name)
        print("DEDI DO: ",list_of_classes[name].childs)
        print("DEDI Z: ",list_of_classes[name].inheritance_from)
        print("ATRIBUTY :", list_of_classes[name].atributes)
        print("METODY :", list_of_classes[name].methods)
        print("OBSAHUJE PURE VIRTUAL :", list_of_classes[name].has_pure)
        print("KONFLIKTY: ", list_of_classes[name].conflicts)
        print("\n\n")
        

#funkce pro tisk metod/atributu pri prepinaci details    
def print_methods_atributes(access,methods_list,atributes_list,spaces,k,print_to_file,name_of_class):
    
    # kontrola zda-li je co tisknout
    if len(methods_list[access]) != 0 or len(atributes_list[access]) != 0:
        spaces += k
        print_to_file += ((spaces * " ")+"<"+access+">\n")
        spaces += k
        
        if len(atributes_list[access]) != 0: # tisk atributu
            print_to_file += ((spaces * " ")+"<attributes>\n")
            spaces += k
            
            for each_atribut in atributes_list[access] :
                if each_atribut[4] != name_of_class :
                    print_to_file += ((spaces * " ")+"<attribute name=\""+each_atribut[1]+"\" type=\""+each_atribut[0]+"\" scope=\""+each_atribut[3]+"\">\n")
                    spaces += k
                    print_to_file += ((spaces * " ")+"<from name=\""+each_atribut[4]+"\" />\n")
                    spaces -= k
                    print_to_file += ((spaces * " ")+"</attribute>\n")
                else :
                    print_to_file += ((spaces * " ")+"<attribute name=\""+each_atribut[1]+"\" type=\""+each_atribut[0]+"\" scope=\""+each_atribut[3]+"\" />\n")
            
            spaces -= k
            print_to_file += ((spaces * " ")+"</attributes>\n")
            
        
        if len(methods_list[access]) != 0 : # tisk metod
            print_to_file += ((spaces * " ")+"<methods>\n")
            spaces += k
            
            for each_method in methods_list[access] :
                print_to_file += ((spaces * " ")+"<method name=\""+each_method[1]+"\" type=\""+each_method[0]+"\" scope=\""+each_method[5]+"\">\n")
                if each_method[6] != name_of_class :
                    spaces += k
                    print_to_file += ((spaces * " ")+"<from name=\""+each_method[6]+"\" />\n")
                    spaces -= k
                if each_method[4] != "normal" :
                    spaces += k
                    print_to_file += ((spaces * " ")+"<virtual pure=\""+each_method[4]+"\" />\n")
                    spaces -= k
                if each_method[2] != None :
                    spaces += k
                    print_to_file += ((spaces * " ")+"<arguments>\n")
                    spaces += k
                    for each_argument in each_method[2]:
                        print_to_file += ((spaces * " ")+"<argument name=\""+each_argument[1]+"\" type=\""+each_argument[0]+"\" />\n")
                    spaces -= k
                    print_to_file += ((spaces * " ")+"</arguments>\n")
                    spaces -= k
                else :
                    spaces += k
                    print_to_file += ((spaces * " ")+"<arguments></arguments>\n")
                    spaces -= k
                
                print_to_file += ((spaces * " ")+"</method>\n")
            
            spaces -= k
            print_to_file += ((spaces * " ")+"</methods>\n")
            
        spaces -= k
        print_to_file += ((spaces * " ")+"</"+access+">\n")
        spaces -= k
        

    return print_to_file

# funkce pro tisk pri prepinaci --details
def print_it_detailed(print_these,list_of_classes,k,conflict):
    
    print_to_file = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" # hlavicka
    
    spaces = 0
    model = False # pokud se tiskne vse
    
    if print_these == None:
        model = True
        print_to_file +=("\n<model>")
        print_these = sorted(list(list_of_classes.keys())) # tisk detailu vsech trid
        spaces += k
    else :
        print_these = [print_these] # tisk detailu zadane tridy
    
    for each_class in print_these: # tisk pro kazdou tridu
        
        my_class = list_of_classes[each_class] # pomocna promenna pro urceni tridy
        
        methods_list = {'public' : [],'protected' : [],'private' : [],'dont show' : []} # seznam metod k tisku podle opravneni
        atributes_list = {'public' : [],'protected' : [],'private' : [],'dont show' : []} # seznam atributu k tisku podle opravneni
        
        for each_method in my_class.methods : # zpracovani seznamu k tisku
            if each_method[3][:11] == 'constructor' :
                methods_list[each_method[3][11:]].append(each_method)
            elif each_method[3][:10] == 'destructor':
                methods_list[each_method[3][10:]].append(each_method)
            else:
                methods_list[each_method[3]].append(each_method)
        for each_atribut in my_class.atributes:
            atributes_list[each_atribut[2]].append(each_atribut)
        
        print_to_file += ("\n"+(spaces * " ")+"<class name=\""+each_class+"\" kind=\""+my_class.has_pure+"\">\n")
        if len(my_class.inheritance_from) != 0 : # pokud odnekud dedim
            spaces += k
            print_to_file += ((spaces * " ")+"<inheritance>\n")
            spaces += k
            
            for each_from in my_class.inheritance_from :
                print_to_file += ((spaces * " ")+"<from name=\""+each_from[0]+"\" privacy=\""+each_from[1]+"\" />\n")
            
            spaces -= k
            print_to_file += ((spaces * " ")+"</inheritance>\n")
            spaces -= k
        
        # tisk metod a atributu podle opravneni
        
        print_to_file = print_methods_atributes("public",methods_list,atributes_list,spaces,k,print_to_file,each_class)
        print_to_file = print_methods_atributes("protected",methods_list,atributes_list,spaces,k,print_to_file,each_class)
        print_to_file = print_methods_atributes("private",methods_list,atributes_list,spaces,k,print_to_file,each_class)
        
        
        if conflict != None: # pokud je aktivni prepinac --conflicts
            if len(my_class.conflicts) != 0:
                spaces += k
                print_to_file += ((spaces * " ")+"<conflicts>\n")
                spaces += k
                for conflict_member in my_class.conflicts.keys():
                    print_to_file += ((spaces * " ")+"<member name=\""+conflict_member+"\">\n")
                    spaces += k
                    for conflict_member_spec in my_class.conflicts[conflict_member]:
                        if len(conflict_member_spec) == 6:
                            print_to_file += ((spaces * " ")+"<class name=\""+conflict_member_spec[4]+"\">\n")
                        else:
                            print_to_file += ((spaces * " ")+"<class name=\""+conflict_member_spec[6]+"\">\n")
                        if len(conflict_member_spec) == 6:
                            spaces += k
                            print_to_file += ((spaces * " ")+"<"+conflict_member_spec[5]+">\n")
                            spaces += k
                            print_to_file += ((spaces * " ")+"<attribute name=\""+conflict_member_spec[1]+"\" type=\""+conflict_member_spec[0]+"\" scope=\""+conflict_member_spec[3]+"\" />\n")
                            spaces -= k
                            print_to_file += ((spaces * " ")+"</"+conflict_member_spec[5]+">\n")
                        elif conflict_member_spec[3] != None: # pokud se nejedna o konflikt v ramci jedne tridy, tisknu podrobnosti
                            spaces += k
                            print_to_file += ((spaces * " ")+"<"+conflict_member_spec[7]+">\n")
                            spaces += k
                            print_to_file += ((spaces * " ")+"<method name=\""+conflict_member_spec[1]+"\" type=\""+conflict_member_spec[0]+"\" scope=\""+conflict_member_spec[5]+"\">\n")
                            if conflict_member_spec[4] != "normal" :
                                spaces += k
                                print_to_file += ((spaces * " ")+"<virtual pure=\""+conflict_member_spec[4]+"\" />\n")
                                spaces -= k
                            if conflict_member_spec[2] != None :
                                spaces += k
                                print_to_file += ((spaces * " ")+"<arguments>\n")
                                spaces += k
                                for each_argument in conflict_member_spec[2]:
                                    print_to_file += ((spaces * " ")+"<argument name=\""+each_argument[1]+"\" type=\""+each_argument[0]+"\" />\n")
                                spaces -= k
                                print_to_file += ((spaces * " ")+"</arguments>\n")
                                spaces -= k
                            else :
                                spaces += k
                                print_to_file += ((spaces * " ")+"<arguments></arguments>\n")
                                spaces -= k
                            print_to_file += ((spaces * " ")+"</method>\n")
                            spaces -= k
                            print_to_file += ((spaces * " ")+"</"+conflict_member_spec[7]+">\n")
                        spaces -= k
                        print_to_file += ((spaces * " ")+"</class>\n")
                    spaces -= k
                    print_to_file += ((spaces * " ")+"</member>\n")
                spaces -= k
                print_to_file += ((spaces * " ")+"</conflicts>\n")
                spaces -= k
            
        print_to_file += ((spaces * " ")+"</class>")
    if model :
        spaces -= k
        print_to_file +=("\n</model>")
            
    return print_to_file
    

# funkce pro tisk stromu dedicnosti, volano rekurzivne
def print_tree(print_these, k, list_of_classes, spaces, printed, print_to_file):
    
    if printed == None:
        print_to_file = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        print_to_file += ("\n<model>\n")
    
    
    spaces += (k * " ")

    for each_class in print_these:
        
        my_class = list_of_classes[each_class]
        
        if len(my_class.inheritance_from) == 0: #base
            if len(my_class.childs) == 0: #bez podelemntu
                print_to_file += (spaces+"<class name=\""+each_class+"\" kind=\""+my_class.has_pure+"\" />\n")
            else :
                print_to_file += (spaces+"<class name=\""+each_class+"\" kind=\""+my_class.has_pure+"\">\n")
                print_to_file = print_tree(sorted(my_class.childs),k,list_of_classes,spaces, each_class, print_to_file)
                print_to_file += (spaces+"</class>\n")
        else:
            if printed == None:
                continue
            if len(my_class.childs) == 0: #bez podelemntu
                print_to_file += (spaces+"<class name=\""+each_class+"\" kind=\""+my_class.has_pure+"\" />\n")
            else :
                print_to_file += (spaces+"<class name=\""+each_class+"\" kind=\""+my_class.has_pure+"\">\n")
                print_to_file = print_tree(sorted(my_class.childs),k,list_of_classes,spaces, each_class, print_to_file)
                print_to_file += (spaces+"</class>\n")
            
    return print_to_file


# funkce pro ziskani dedenych clenu
def get_inheritance(list_of_classes,whole_class,name,conflicts):
    
    my_regex = 'class\s+' + name + '\s*:(.|\n)*?{' # kontrola zda-li se jedna o dedicnost
    
    public = False
    private = True
    protected = False
    
    if re.search(my_regex,whole_class) != None:
        inheritance_classes = re.search(my_regex,whole_class).group(0)
        inheritance_classes = inheritance_classes[:-1] # seznam trid, ze kterych dedim
        inheritance_classes = inheritance_classes.replace(name,'',1)
        splitted = re.split('\s|,|:',inheritance_classes)
        for j in splitted: # zjisteni opravneni a jmena tridy
            is_here = False
            if re.match('\w+',j) and j != 'class':
                if j == 'public':
                    public = True
                elif j == 'private':
                    private = True
                elif j == 'protected':
                    protected = True
                else:
                    if j == name:
                        sys.exit(4)
                    for key in list_of_classes.keys():
                        if key == j:
                            is_here = True
                    if public and is_here and list_of_classes[j].defined:
                        for base in list_of_classes[name].inheritance_from:
                            if base[0] == j:
                                sys.exit(4)
                        list_of_classes[name].inheritance_from.append((j,'public'))
                        public = False
                    elif protected and is_here and list_of_classes[j].defined:
                        for base in list_of_classes[name].inheritance_from:
                            if base[0] == j:
                                sys.exit(4)
                        protected = False
                        list_of_classes[name].inheritance_from.append((j,'protected'))
                    elif private and is_here and list_of_classes[j].defined:
                        for base in list_of_classes[name].inheritance_from:
                            if base[0] == j:
                                sys.exit(4)
                        list_of_classes[name].inheritance_from.append((j,'private'))
                    else:
                        sys.exit(4)
                        
                    list_of_classes[j].childs.append(name) # pridani do seznamu trid do kterych dedim

    
    for base_class in list_of_classes[name].inheritance_from : # pro kazdou tridu ze ktere dedim
        
        for add_atribute in list_of_classes[base_class[0]].atributes : # ziskani atributu
            conflict = True
            add = True
            
            for own_atribute in list_of_classes[name].own_atributes :
                if own_atribute[1] == add_atribute[1] :
                    add = False
                    conflict = False
                    break
            
            if conflicts != None: # pouze pro prepinac --conflicts
                skip = False
                for conflict_member in list_of_classes[name].conflicts.keys(): 
                    if conflict_member == add_atribute[1]: # kontrola zda-li se nejedna o konflitni clen
                        same_class = False
                        for each_conflict in list_of_classes[name].conflicts[conflict_member]:
                            if len(each_conflict) == 6:
                                if each_conflict[4] == add_atribute[4]:
                                    each_conflict[4] = None
                                    skip = True
                                    same_class = True
                                    break
                            else:
                                if each_conflict[6] == add_atribute[4]:
                                    each_conflict[6] = None
                                    skip = True
                                    same_class = True
                                    break
                        if same_class == False:
                            list_of_classes[name].conflicts[conflict_member].append(add_atribute)
                        skip = True
                        break
                if skip:
                    continue
                    
            if conflict : # pokud je mozny konflikt
                for from_all in list_of_classes[name].atributes:  
                    if from_all[1] == add_atribute[1] : # porovnam se vsemi atributy co mam
                        if conflicts == None:
                            sys.exit(21)
                        else: # zalozim novy konflikt
                            list_of_classes[name].conflicts[from_all[1]] = []
                            list_of_classes[name].conflicts[from_all[1]].append(add_atribute)
                            if from_all[4] != add_atribute[4]:
                                list_of_classes[name].conflicts[from_all[1]].append(from_all)
                            list_of_classes[name].atributes.remove(from_all)
                            add = False
                                    
                for from_all in list_of_classes[name].methods: # porovnam se vsemi metodami co mam
                    if from_all[1] == add_atribute[1] :
                        if conflicts == None:
                            sys.exit(21)
                        else: # zalozim novy konflikt
                            list_of_classes[name].conflicts[from_all[1]] = []
                            list_of_classes[name].conflicts[from_all[1]].append(add_atribute)
                            if from_all[6] != add_atribute[4]:
                                list_of_classes[name].conflicts[from_all[1]].append(from_all)
                            list_of_classes[name].methods.remove(from_all)
                            add = False
                               
            
            if add: # pridam podle opravneni
                if base_class[1] == "public" :
                    if add_atribute[2] == "public" :
                        list_of_classes[name].atributes.append((add_atribute[0],add_atribute[1],"public",add_atribute[3],add_atribute[4],add_atribute[5]))
                    elif add_atribute[2] == "protected" :
                        list_of_classes[name].atributes.append((add_atribute[0],add_atribute[1],"protected",add_atribute[3],add_atribute[4],add_atribute[5]))
                    elif add_atribute[2] == "private" :
                        list_of_classes[name].atributes.append((add_atribute[0],add_atribute[1],"dont show",add_atribute[3],add_atribute[4],add_atribute[5]))
                    elif add_atribute[2] == "dont show" :
                        list_of_classes[name].atributes.append((add_atribute[0],add_atribute[1],"dont show",add_atribute[3],add_atribute[4],add_atribute[5]))
                elif base_class[1] == "protected" :
                    if add_atribute[2] == "public" :
                        list_of_classes[name].atributes.append((add_atribute[0],add_atribute[1],"protected",add_atribute[3],add_atribute[4],add_atribute[5]))
                    elif add_atribute[2] == "protected" :
                        list_of_classes[name].atributes.append((add_atribute[0],add_atribute[1],"protected",add_atribute[3],add_atribute[4],add_atribute[5]))
                    elif add_atribute[2] == "private" :
                        list_of_classes[name].atributes.append((add_atribute[0],add_atribute[1],"dont show",add_atribute[3],add_atribute[4],add_atribute[5]))
                    elif add_atribute[2] == "dont show" :
                        list_of_classes[name].atributes.append((add_atribute[0],add_atribute[1],"dont show",add_atribute[3],add_atribute[4],add_atribute[5]))
                elif base_class[1] == "private" :
                    if add_atribute[2] == "public" :
                        list_of_classes[name].atributes.append((add_atribute[0],add_atribute[1],"private",add_atribute[3],add_atribute[4],add_atribute[5]))
                    elif add_atribute[2] == "protected" :
                        list_of_classes[name].atributes.append((add_atribute[0],add_atribute[1],"private",add_atribute[3],add_atribute[4],add_atribute[5]))
                    elif add_atribute[2] == "private" :
                        list_of_classes[name].atributes.append((add_atribute[0],add_atribute[1],"dont show",add_atribute[3],add_atribute[4],add_atribute[5]))
                    elif add_atribute[2] == "dont show" :
                        list_of_classes[name].atributes.append((add_atribute[0],add_atribute[1],"dont show",add_atribute[3],add_atribute[4],add_atribute[5]))
                        
                        
        for add_method in list_of_classes[base_class[0]].methods : # pro vsechny metody, ktere chci pridat
            conflict = True
            add = True
            
            for own_method in list_of_classes[name].own_methods : # pokud je metoda redefinovana ve sve tride, nemusim resit konflikty

                if own_method[1] == add_method[1] and own_method[2] == add_method[2]: # nazev i argumenty jsou stejne, nebudu pridavat
                    add = False
                    conflict = False
                    break
                
                if own_method[2] != None and add_method[2] != None: # pokud jsou argumenty ruzne od None
                    if own_method[1] == add_method[1] and len(own_method[2]) == len(add_method[2]): # pokud je nazev a pocet argumentu stejny, muze se jednat o redefinici
                        
                        zipped = zip(own_method[2],add_method[2]) # budu porovnavat dvojice argumentu v jejich poradi
                        same = True
                        for each_args_type in zipped: # kazda dvojice
                            if each_args_type[0][0] != each_args_type[1][0]: # pokud se lisi nejde o redefinici
                                same = False
                        
                        if same == True : # pokud byly stejne, tak se jedna o redefinici
                            conflict = False
                            add = False # neni nutne ji pridavat
            
            if conflicts != None: # kontrola konfliktnich clenu
                skip = False
                for conflict_member in list_of_classes[name].conflicts.keys():
                    if conflict_member == add_method[1]:
                        same_class = False
                        for each_conflict in list_of_classes[name].conflicts[conflict_member]:
                            if len(each_conflict) == 6:
                                if each_conflict[4] == add_method[4]:
                                    skip = True
                                    same_class = True
                                    break
                            else:
                                if each_conflict[6] == add_method[4]:
                                    if each_conflict[2] != None and add_method[2] != None: # pokud jsou argumenty ruzne od None
                                        if each_conflict[1] == add_method[1] and len(each_conflict[2]) == len(add_method[2]): # pokud je nazev a pocet argumentu stejny, muze se jednat o redefinici
                                            
                                            zipped = zip(each_conflict[2],add_method[2]) # budu porovnavat dvojice argumentu v jejich poradi
                                            same = True
                                            for each_args_type in zipped: # kazda dvojice
                                                if each_args_type[0][0] != each_args_type[1][0]: # pokud se lisi nejde o redefinici
                                                    same = False
                                            
                                            if same == True:
                                                skip = True
                                                same_class = True
                        
                        if same_class == False:
                            list_of_classes[name].conflicts[conflict_member].append(add_method)
                        skip = True
                        break
                if skip:
                    continue
                            
            for from_all in list_of_classes[name].atributes: # kontrola na stributy
                if from_all[1] == add_method[1] :
                    if conflicts == None:
                        sys.exit(21)
                    else:
                            list_of_classes[name].conflicts[from_all[1]] = []
                            list_of_classes[name].conflicts[from_all[1]].append(add_method)
                            if from_all[4] != add_method[6]:
                                list_of_classes[name].conflicts[from_all[1]].append(from_all)
                            list_of_classes[name].atributes.remove(from_all)
                            add = False
                                
                
            if conflict : # pri moznosti konfliktu
                for from_all in list_of_classes[name].methods: # tentokrat budu porovnavat pro vsechny do ted pridane metody

                    
                    if from_all[1] == add_method[1] and from_all[2] == add_method[2]: # nazev i argumenty jsou stejne, jedna se o konflikt
                        if conflicts == None:
                            sys.exit(21)
                        else: # zalozim konflikt
                            list_of_classes[name].conflicts[from_all[1]] = []
                            list_of_classes[name].conflicts[from_all[1]].append(add_method)
                            if from_all[6] != add_method[6]:
                                list_of_classes[name].conflicts[from_all[1]].append(from_all)
                            list_of_classes[name].methods.remove(from_all)
                            add = False
                            break
                    
                    if from_all[2] != None and add_method[2] != None: # pokud jsou argumenty ruzne od None
                        if from_all[1] == add_method[1] and len(from_all[2]) == len(add_method[2]): # pokud je nazev a pocet argumentu stejny, muze se jednat o konflikt
                            
                            zipped = zip(from_all[2],add_method[2]) # budu porovnavat dvojice argumentu v jejich poradi
                            same = True
                            for each_args_type in zipped: # kazda dvojice
                                if each_args_type[0][0] != each_args_type[1][0]: # pokud se lisi nejde o konflikt
                                    same = False
                            
                            if same == True : # pokud byly stejne, tak se jedna o konflikt
                                if conflicts == None:
                                    sys.exit(21)
                                else: # zalozim konflikt
                                    list_of_classes[name].conflicts[from_all[1]] = []
                                    list_of_classes[name].conflicts[from_all[1]].append(add_method)
                                    if from_all[6] != add_method[6]:
                                        list_of_classes[name].conflicts[from_all[1]].append(from_all)
                                    list_of_classes[name].methods.remove(from_all)
                                    add = False
                        
                        
            if add: # pridam podle opravneni
                    
                if add_method[4] == "yes" :
                    list_of_classes[name].has_pure = "abstract"
                    if add_method[3] == "private":
                        list_of_classes[name].methods.append((add_method[0],add_method[1],add_method[2],"private",add_method[4],add_method[5],add_method[6],add_method[7]))
                        break
                    list_of_classes[name].methods.append((add_method[0],add_method[1],add_method[2],base_class[1],add_method[4],add_method[5],add_method[6],add_method[7]))
                    break
                    
                if base_class[1] == "public" :
                    if add_method[3] == "public" :
                        list_of_classes[name].methods.append((add_method[0],add_method[1],add_method[2],"public",add_method[4],add_method[5],add_method[6],add_method[7]))
                    elif add_method[3] == "protected" :
                        list_of_classes[name].methods.append((add_method[0],add_method[1],add_method[2],"protected",add_method[4],add_method[5],add_method[6],add_method[7]))
                    elif add_method[3] == "private" :
                        list_of_classes[name].methods.append((add_method[0],add_method[1],add_method[2],"dont show",add_method[4],add_method[5],add_method[6],add_method[7]))
                    elif add_method[3] == "dont show" :
                        list_of_classes[name].methods.append((add_method[0],add_method[1],add_method[2],"dont show",add_method[4],add_method[5],add_method[6],add_method[7]))
                elif base_class[1] == "protected" :
                    if add_method[3] == "public" :
                        list_of_classes[name].methods.append((add_method[0],add_method[1],add_method[2],"protected",add_method[4],add_method[5],add_method[6],add_method[7]))
                    elif add_method[3] == "protected" :
                        list_of_classes[name].methods.append((add_method[0],add_method[1],add_method[2],"protected",add_method[4],add_method[5],add_method[6],add_method[7]))
                    elif add_method[3] == "private" :
                        list_of_classes[name].methods.append((add_method[0],add_method[1],add_method[2],"dont show",add_method[4],add_method[5],add_method[6],add_method[7]))
                    elif add_method[3] == "dont show" :
                        list_of_classes[name].methods.append((add_method[0],add_method[1],add_method[2],"dont show",add_method[4],add_method[5],add_method[6],add_method[7]))
                elif base_class[1] == "private" :
                    if add_method[3] == "public" :
                        list_of_classes[name].methods.append((add_method[0],add_method[1],add_method[2],"private",add_method[4],add_method[5],add_method[6],add_method[7]))
                    elif add_method[3] == "protected" :
                        list_of_classes[name].methods.append((add_method[0],add_method[1],add_method[2],"private",add_method[4],add_method[5],add_method[6],add_method[7]))
                    elif add_method[3] == "private" :
                        list_of_classes[name].methods.append((add_method[0],add_method[1],add_method[2],"dont show",add_method[4],add_method[5],add_method[6],add_method[7]))
                    elif add_method[3] == "dont show" :
                        list_of_classes[name].methods.append((add_method[0],add_method[1],add_method[2],"dont show",add_method[4],add_method[5],add_method[6],add_method[7]))
                    
    #print("CLASS: " + i)
    #print(id(list_of_classes[i]))
    #print(list_of_classes[i].inheritance_from)


def get_params(argv):
    
    my_params = params()
    # prommene, pokud by prepinac nalezen
    in_b = True
    out_b = True
    pretty_b = True
    details_b = True
    search_b = True
    conflicts_b = True
    

    
    for opt in sys.argv[1:]:
        if opt == '--help' and len(sys.argv) == 2:
            print("___________________________________________________________________Napoveda_____________________________________________________________\n")
            print("Skript pro zpracovani hlavickovych souboru jazyka C++ a naslednou analyzu dedicnosti\n")
            print("Pouziti:\n")
            print("--help \t\t\t vypise napovedu\n")
            print("--input=filename\t cesta ke vstupnimu souboru s hlavickovym souborem\n")
            print("--output=filename\t cesta k vystupnimu souboru\n")
            print("--pretty-xml[=k]\t vystupni soubor bude odsazen o k mezer, pri nezadani je k 4\n")
            print("--details[=class]\t detailni vypis tridy \"class\", pokud neni zadano, tak je vypsan detailni vypis o vsech tridach\n")
            print("--search=XPATH\t\t do vystupniho souboru se vypisi vysledek vyhledavani podle vyrazu XPATH\n")
            print("--conflicts\t\t pokud je zadan spolecne s prepinacem --details, tak vypise konfliktni clen do detailniho vypisu namisto ukonceni skriptu\n")
            sys.exit(0)
        elif opt[:8] == '--input=' and in_b:
            my_params.input_file = opt[8:]
            if len(my_params.input_file) == 0 :
                sys.exit(1)
            in_b = False
        elif opt[:9] == '--output=' and out_b:
            my_params.output_file = opt[9:]
            if len(my_params.output_file) == 0 :
                sys.exit(1)
            out_b = False
        elif opt[:13] == '--pretty-xml=' and pretty_b:
            my_params.k = int(opt[13:])
            if len(opt[13:]) == 0 :
                sys.exit(1)
            pretty_b = False
        elif opt == '--pretty-xml' and pretty_b:
            my_params.k = 4
            pretty_b = False
        elif opt[:10] == '--details=' and details_b:
            my_params.detailed_class = opt[10:]
            if len(my_params.detailed_class) == 0 :
                sys.exit(1)
            my_params.detailed = True
            details_b = False
        elif opt[:9] == '--details' and details_b:
            my_params.detailed = True
            details_b = False
        elif opt == '--conflicts' and conflicts_b:
            my_params.conflicts = True
            conflicts_b = False
        elif opt[:9] == '--search=' and search_b:
            my_params.search_xpath = opt[9:]
            if len(my_params.search_xpath) == 0 :
                sys.exit(1)
            search_b = False
        else:
            sys.exit(1)
            
    if in_b and out_b and pretty_b and details_b and search_b and conflicts_b and len(sys.argv) != 1:
        sys.exit(1)
        
    if conflicts_b == False:
        if details_b == True:
            sys.exit(1)
        
    

    
    return my_params

def get_xpath(print_to_file,k,xpath_exp): # prace s vyrazy XPATH
    print_to_file=print_to_file[print_to_file.find('\n')+1:] # odstraneni hlavicky
    
    result = etree.Element("result") # zaozeni noveho strommu
    root = etree.fromstring(print_to_file) # prevod ze stringu na strom
    xpr = root.xpath(xpath_exp) # vyhledani vyrazu
    
    if len(xpr) != 0 : # pri nalzeu
        if isinstance(xpr[0],etree._Element) :
            for all_ss in xpr : # pokud je vysledkem element, pridavam jako potomky
                result.append(all_ss)
            string = (etree.tostring(result,encoding="utf-8", pretty_print=True)).decode("utf-8") # prevod stromu do stringu
            string = re.sub('<result>',"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<result>\n",string,1) # nahrazeni hlavicky
            string = re.sub((k*' ')+'( *)','\g<1>',string) # oprava odsazeni
        else : # pokud je vysledek string, tisknu
            string = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<result>\n"
            for all_ss in sorted(xpr) :
                string += (k*' ')+all_ss+"\n"
            string += "</result>"
        
    return string # vracim string

def last_control(list_of_classes,name): # kontrola na neopravnene pretizeni metod
    
    for each_method in list_of_classes[name].methods: # pokud je metoda stejneho jmena jako atribut, chyba
        for each_atribut in list_of_classes[name].atributes:
            if each_atribut[1] == each_method[1]:
                sys.exit(4)
        fail = False
        
        for each_method2 in list_of_classes[name].methods: # pokud je metoda stejneoh nazvu
            if each_method != each_method2:
                if each_method2[1] == each_method[1]:
                    if each_method2[2] == each_method[2]: # nazev i argumenty jsou stejne, nebudu pridavat
                        fail = True
                        break
                
                if each_method2[2] != None and each_method[2] != None : # pokud jsou argumenty ruzne od None
                    if len(each_method2[2]) == len(each_method[2]): # pokud je nazev a pocet argumentu stejny, muze se jednat o redefinici
                        
                        zipped = zip(each_method2[2],each_method[2]) # budu porovnavat dvojice argumentu v jejich poradi
                        same = True
                        for each_args_type in zipped: # kazda dvojice
                            if each_args_type[0][0] != each_args_type[1][0]: # pokud se lisi nejde o redefinici
                                same = False
                        if same == True : # pokud byly stejne, tak se jedna o eopravnene pretizeni
                            fail = True
        
        if fail:
            sys.exit(4)
                        
def main(): # main funkce
    my_params = params()
    my_params = get_params(sys.argv[1:])
    list_of_classes = get_classes(my_params)
    
    if list_of_classes == None and (my_params.detailed_class == None or my_params.detailed == None): # pokud neni cesta a je prazdny soubor
        
        print_to_file = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<model/>"
        
        if my_params.output_file != None :
            try:
                my_file = open(my_params.output_file, 'w')
                my_file.write(print_to_file)
                my_file.close()
            except IOError:
                sys.exit(3)
        else :
            sys.stdout.write(print_to_file)
            
        sys.exit(0)
            
    bs = False
    
    if list_of_classes == None and my_params.detailed_class != None: # kontrola zda je pozadovana trida v seznamu
        bs = True
        for ch_d in list_of_classes.keys():
            if ch_d == my_params.detailed_class:
                bs = False
            
    
    for ch_d in list_of_classes.keys(): # kontrola zda jsou tridy definovane
        if list_of_classes[ch_d].defined == False:
            sys.exit(4)
    #print_all(list_of_classes)
    print_to_file = ""

    if bs == True: # tisk
        print_to_file("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")
    elif my_params.detailed :
        print_to_file = print_it_detailed(my_params.detailed_class,list_of_classes,my_params.k,my_params.conflicts)
    else :
        print_to_file = print_tree(sorted(list(list_of_classes.keys())),my_params.k,list_of_classes,"",None,print_to_file)
        print_to_file += ("</model>")
        
    if my_params.search_xpath != None :
        print_to_file = get_xpath(print_to_file,my_params.k,my_params.search_xpath)
    
    if my_params.output_file != None :
        try:
            my_file = open(my_params.output_file, 'w')
            my_file.write(print_to_file)
            my_file.close()
        except IOError:
            sys.exit(3)
    else :
        sys.stdout.write(print_to_file)
    
if __name__ == "__main__":
    main()
