package PDB.controller;

import PDB.entity.PDBArea;
import PDB.entity.PDBDefaultEntity;
import PDB.entity.PDBvehicle;
import PDB.service.AreaService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Collection;

/**
 * Třida pro zpracovani requestu arealu
 */
@RestController
@RequestMapping("/area")
public class AreaController {

    @Autowired
    private AreaService areaService;

    /**
     * Ziskani vsechny arealy
     * @return
     */
    @RequestMapping(method = RequestMethod.GET)
    public ResponseEntity<Collection<PDBArea>> getAllAreas(){
        return areaService.getAllAreas();
    }

    /**
     * Vytvori novou areu
     * @param area
     * @return
     */
    @RequestMapping(method = RequestMethod.POST)
    public ResponseEntity<PDBArea> createArea(@RequestBody PDBDefaultEntity area){
        return areaService.createArea(area);
    }

    /**
     * Upravi novou areau
     * @param area
     * @return
     */
    @RequestMapping(method = RequestMethod.PUT)
    public ResponseEntity<PDBArea> updateArea(@RequestBody PDBDefaultEntity area){
        return areaService.updateArea(area);
    }

    /**
     * Odstraní areu
     * @param id
     * @return
     */
    @RequestMapping(path="/{id}" , method = RequestMethod.DELETE)
    public ResponseEntity<Void> removeArea(@PathVariable("id") Integer id){
        return areaService.removeArea(id);
    }

    /**
     * ziskani vychozich entit pro kresleni
     * @return
     */
    @RequestMapping(path = "/default", method = RequestMethod.GET)
    public ResponseEntity<Collection<PDBDefaultEntity>> getDefaultEntities(){
        return areaService.getDefaultEntities();
    }

    /**
     * Ziskani prazdnych slotu
     * @return
     */
    @RequestMapping(path ="/empty", method = RequestMethod.GET)
    public ResponseEntity<Collection<Integer>> getEmptySpots(){
        return areaService.getEmptySpots();
    }

    /**
     * Ziskani velikosti oblasti
     * @param id
     * @return
     */
    @RequestMapping(path = "/space/{id}", method = RequestMethod.GET)
    public ResponseEntity<Double> getAreaSpace(@PathVariable("id") Integer id){
        return areaService.getAreaSpace(id);
    }

    /**
     * Vzdalenost dvou oblasti
     * @param id
     * @return
     */
    @RequestMapping(path = "/length/{id}", method = RequestMethod.GET)
    public ResponseEntity<Double> getAreaLength(@PathVariable("id") Integer id){
        return areaService.getAreaLength(id);
    }

    /**
     * Ziskani parkovacich mist
     * @param id
     * @return
     */
    @RequestMapping(path = "/parkingSpots/{id}", method = RequestMethod.GET)
    public ResponseEntity<Collection<PDBArea>> getParkingSpots(@PathVariable("id") Integer id){
        return areaService.getParkingSports(id);
    }

    /**
     * Ziskani prumerne ceny vozidel v arealu
     * @param id
     * @return
     */
    @RequestMapping(path = "/averagePrice/{id}", method = RequestMethod.GET)
    public ResponseEntity<Double> getAveragePrice(@PathVariable("id") Integer id){
        return areaService.getAveragePrice(id);
    }

    /**
     * Vzdalenoust dvou oblasti
     * @param idA
     * @param idB
     * @return
     */
    @RequestMapping(path = "/distance", method = RequestMethod.GET)
    public ResponseEntity<Double> getDistance(@RequestParam(value="idA") Integer idA, @RequestParam(value="idB") Integer idB) {
        return areaService.getAreaDistance(idA, idB);
    }

    /**
     * Ziskani vozidel v oblasti
     * @param idA
     * @param idB
     * @return
     */
    @RequestMapping(path = "/vehicles", method = RequestMethod.GET)
    public ResponseEntity<Collection<PDBvehicle>> getVehiclesInAreas(@RequestParam(value="idA") Integer idA, @RequestParam(value="idB") Integer idB) {
        return areaService.getVehiclesInAreas(idA, idB);
    }







    }
