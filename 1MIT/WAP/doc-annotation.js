/**
 * WAP: Anotace dokumentu 2017/2018
 * Autor: David Kolečkář - xkolec07
 */

(function(){
    //nacist cely dokument pro pracis jednotlivymi elementy
    document.addEventListener("DOMContentLoaded", function(event) { 

        //inicializace promenych
        var prevtarget = null;
        var jsonStr = '{"notes":[]}';
        var obj = JSON.parse(jsonStr);
        var allEll = document.getElementsByTagName("*");

        //vytvoreni divu pro zobrazeni velikosti
        createSize();
        //vytvoreni dialogu pro pridani poznamky
        createAddDialog();
        //vytvoreni dialogu pro zobrazeni poznamek
        createShowDialog();
        //nalezeni vsech elementu s atributem title 
        findTitleAtribute(allEll,obj);

        // promene s kterymi pracuji listenery
        var tmp = document.getElementsByClassName("doc-annotation-size")[0];
        var modal = document.getElementsByClassName("doc-annotation-modal")[0];
        var modal1 = document.getElementsByClassName("doc-annotation-modal")[1];
        var close = document.getElementsByClassName("doc-annotation-close")[0];
        var close1 = document.getElementsByClassName("doc-annotation-close")[1];
        var save = document.getElementsByClassName("doc-annotation-button")[0];

     
        document.addEventListener("mousemove", function(e) {
            e = e || window.event;
            x = e.clientX;
            y = e.clientY;
            var target = elementsFromPoint(x,y);

            if(tmp != target && prevtarget != target && target.tagName !== "HTML" 
            && target.tagName !== "BODY" && target != modal && target != close 
            && target.className != "doc-annotation-icon" && target != modal1 && target != close1){
                tmp.onclick = function(){
                    modal.style.display = "block";
                    var note = document.getElementsByClassName("doc-annotation-input")[0];
                    //kliknuti na save v dialogovem okne
                    save.onclick = function(){
                        //pokud je pole pro poznamku prazdne - zobraz varovnou hlasku
                        if(note.value.toString() === ""){
                            note.placeholder = "Insert a note!";
                            return false;
                        }
                        //jinak ulozit poznamku
                        else{
                            saveNote(obj,target,allEll);
                            modal.style.display = "none";
                            note.value = "";
                            var id = getId(target,allEll);
                            //pokud neexistuje ikona - vytvor ji
                            var pom = document.getElementById("doc-annotation-icon-"+id.toString());
                            if(pom === null){
                                createIcon(id,allEll,obj);
                            }
                        }
                    }//end save click
                    //zavreni dialogoveho okna
                    close.onclick = function(){
                        note.value = "";
                        modal.style.display = "none";
                    }
                }

                tmp.style.left = getOffset(target).left.toString()+"px";
                tmp.style.top =  getOffset(target).top.toString()+"px";
                tmp.style.width = target.offsetWidth.toString()+"px";
                tmp.style.height  = target.offsetHeight.toString()+"px";
                tmp.style.lineHeight =  target.offsetHeight.toString()+"px";
                tmp.innerHTML = elementSize(target);
                tmp.style.display = "block";
                
            }
            prevtarget = target;
        });
       
    });//end DOMContentLoaded
})();

/*
 * Funkce pro ziskani elementu na dane pozici x,y
 */
function elementsFromPoint(x,y) {
    var elements = [];
    var prevPointer = []; 
    var current, i, d;

    while ((current = document.elementFromPoint(x,y)) && elements.indexOf(current) === -1 && current != null){ 
        elements.push(current);
        prevPointer.push({
                value: current.style.getPropertyValue('pointer-events'),
                priority: current.style.getPropertyPriority('pointer-events')
        });
          
        current.style.setProperty('pointer-events', 'none', 'important'); 
    }

    for(i = prevPointer.length; d=prevPointer[--i]; ) {
        elements[i].style.setProperty('pointer-events', d.value?d.value:'', d.priority); 
    }

    if(elements[0].className === "doc-annotation-size")
        return  elements[1];
    else
        return elements[0];
}

/*
 * Funkce pro ziskani velikosti elementu
 */
function elementSize(e){
    return e.offsetHeight.toString() + " x " +e.offsetWidth.toString();
}

/*
 * Funkce pro ziskani pozice TOP, LEFT elementu
 */
function getOffset(e) {
    e = e.getBoundingClientRect();
    return {
        left: e.left + window.scrollX,
        top: e.top + window.scrollY
    }
}

/*
 * Funkce pro vytvoreni DIV elementu, ktery zobrazuje ikonu znazornujici 
 * ulozenou poznamku k danemu elementu
 */
function createIcon(id,allEll,obj){
    var e = allEll[id];

    var icon = document.createElement("div");
    icon.className  = "doc-annotation-icon";
    var right = parseInt(e.offsetWidth) + parseInt(getOffset(e).left) - 12;
    icon.style.left = right.toString()+"px";
    icon.style.top =  getOffset(e).top.toString()+"px";
    icon.id = "doc-annotation-icon-"+id.toString();
    document.body.appendChild(icon); 

    var modal = document.getElementsByClassName("doc-annotation-modal")[1];
    icon.onclick = function(e){
        modal.style.display = "block";
        var str = e.target.id.toString();
        var n = str.lastIndexOf("-"); 
        var id =  str.substr(n+1,str.length);
        
        for (var i = 0; i < obj.notes.length; i++) {
            if(obj.notes[i].id === id){
                var p = document.createElement("p");
                p.className = "doc-annotation-p";
                p.style.background = obj.notes[i].color;
                p.innerHTML = "<b>Note: </b>" + obj.notes[i].note + "<b> Date: </b>" + obj.notes[i].date;
                document.getElementsByClassName("doc-annotation-modal-content")[1].appendChild(p);
            }
        }
    }

    var close = document.getElementsByClassName("doc-annotation-close")[1];
    close.onclick = function(){
        modal.style.display = "none";
        var elements = document.getElementsByClassName("doc-annotation-p");
        while(elements.length > 0){
            elements[0].parentNode.removeChild(elements[0]);
        }
    }
}

/*
 * Funkce pro vytvoreni DIV elementu, ktery zobrazuje jeho velikost
 */
function createSize(){
    var sizediv = document.createElement("div");
    sizediv.className  = "doc-annotation-size";
    document.body.appendChild(sizediv); 
}

/*
 * Funkce pro vytvoreni dialogoveho okna pro pridani poznamky
 */
function createAddDialog(){
    var modal = document.createElement("div");
    modal.className  = "doc-annotation-modal";
    document.body.appendChild(modal);

    var modalcontent = document.createElement("div");
    modalcontent.className  = 'doc-annotation-modal-content';
    document.getElementsByClassName("doc-annotation-modal")[0].appendChild(modalcontent);

    var cl = document.createElement("span");
    cl.className  = "doc-annotation-close";
    cl.innerHTML = "&times;"
    document.getElementsByClassName("doc-annotation-modal-content")[0].appendChild(cl);

    var h = document.createElement("h2");
    h.innerHTML = "Add note:"
    document.getElementsByClassName("doc-annotation-modal-content")[0].appendChild(h);

    var input = document.createElement("input");
    input.type = "text";
    input.className = "doc-annotation-input";
    input.placeholder = "Your note";
    document.getElementsByClassName("doc-annotation-modal-content")[0].appendChild(input);

    var select = document.createElement("select");
    select.name = "color";
    select.className = "doc-annotation-input";
    document.getElementsByClassName("doc-annotation-modal-content")[0].appendChild(select);

    var red = document.createElement("option");
    red.value = "#ff9999"; red.innerHTML = "Red";
    document.getElementsByClassName("doc-annotation-input")[1].appendChild(red);
    var green = document.createElement("option");
    green.value = "#adebad"; green.innerHTML = "Green";
    document.getElementsByClassName("doc-annotation-input")[1].appendChild(green);
    var blue = document.createElement("option");
    blue.value = "#b3e0ff"; blue.innerHTML = "Blue";
    document.getElementsByClassName("doc-annotation-input")[1].appendChild(blue);

    var but = document.createElement("input");
    but.type = "submit";
    but.className = "doc-annotation-button";
    but.value = "Save note";
    document.getElementsByClassName("doc-annotation-modal-content")[0].appendChild(but);
}

/*
 * Funkce pro ulozeni poznamek s atributem title
 */
function findTitleAtribute(allEll,obj){
    for (var i = 0; i < allEll.length; i++) {
        if(allEll[i].getAttribute("title") != null){
            var note = allEll[i].getAttribute("title").toString();
            obj.notes.push({"id":i.toString(),"note":note,"color":"white","date":"unkown"});
            createIcon(i,allEll,obj);
        }
    }
}

/*
 * Funkce pro ziskani id vybraneho elementu
 */
function getId(e,allEll){
    var id;
    for (var i = 0; i < allEll.length; i++) {
        if(allEll[i] === e){
            id = i;
        }
    }
    return id;  
}

/*
 * Funkce pro ulozeni poznamky do pole JSON
 */
function saveNote(obj,e,allEll){
    var id = getId(e,allEll).toString();
    var note = document.getElementsByClassName("doc-annotation-input")[0].value.toString();
    var currentdate = new Date(); 
    var datetime =  currentdate.getDate() + "."
                    + (currentdate.getMonth()+1)  + "." 
                    + currentdate.getFullYear() + " "  
                    + currentdate.getHours() + ":"  
                    + currentdate.getMinutes() + ":" 
                    + currentdate.getSeconds();
    var color = document.getElementsByClassName("doc-annotation-input")[1].value.toString();

    obj.notes.push({"id":id,"note":note,"color":color,"date":datetime.toString()});
}

/*
 * Funkce pro vytvoreni dialogoveho okna pro zobrazeni poznamek
 */
function createShowDialog(){
    var m = document.createElement("div");
    m.className  = "doc-annotation-modal";
    document.body.appendChild(m);

    var c = document.createElement("div");
    c.className  = 'doc-annotation-modal-content';
    document.getElementsByClassName("doc-annotation-modal")[1].appendChild(c);

    var s = document.createElement("span");
    s.className  = "doc-annotation-close";
    s.innerHTML = "&times;"
    document.getElementsByClassName("doc-annotation-modal-content")[1].appendChild(s);

    var h = document.createElement("h2");
    h.innerHTML = "Notes:"
    document.getElementsByClassName("doc-annotation-modal-content")[1].appendChild(h);
}


