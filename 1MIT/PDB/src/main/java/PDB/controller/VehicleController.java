package PDB.controller;

import PDB.entity.PDBUserPrice;
import PDB.entity.PDBUserVehicle;
import PDB.entity.PDBvehicle;
import PDB.service.VehicleService;
import oracle.sql.DATE;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Collection;

/**
 * Třida pro obsluhu requestu vozidla
 */
@RestController
@RequestMapping("/vehicles")
public class VehicleController {

    @Autowired
    private VehicleService vehicleService;

    /**
     * Ziskani vsech vozidel
     * @param sold
     * @return
     */
    @RequestMapping(method = RequestMethod.GET)
    public ResponseEntity<Collection<PDBvehicle>> getAllVehicles(
            @RequestParam(value="sold") Boolean sold
    )
    {
        return vehicleService.getAllVehicles(sold);
    }

    /**
     * Ziskani editovanych vozidel
     * @param from
     * @param to
     * @return
     */
    @RequestMapping(path = "/edited", method = RequestMethod.GET)
    public ResponseEntity<Collection<PDBUserVehicle>> getEditedVehicles(@RequestParam(value="from") DATE from, @RequestParam(value="to") DATE to){
        return vehicleService.getEditedVehicles(from, to);
    }

    /**
     * Vytvoreni noveho vozidla
     * @param vehicle
     * @return
     */
    @RequestMapping(method = RequestMethod.POST)
    public ResponseEntity<PDBvehicle> createVehicle(@RequestBody PDBvehicle vehicle){
        return vehicleService.createVehicle(vehicle);
    }

    /**
     * Ziskani vozidla dle ID
     * @param id
     * @return
     */
    @RequestMapping(value = "/{id}", method = RequestMethod.GET)
    public ResponseEntity<PDBvehicle> getVehicleById(@PathVariable("id") long id){
        return vehicleService.getVehicleById(id);
    }

    /**
     * Ziskani cen vozidla dle ID
     * @param id
     * @return
     */
    @RequestMapping(value = "/{id}/prices", method = RequestMethod.GET)
    public ResponseEntity<Collection<PDBUserPrice>> getVehiclePrices(@PathVariable("id") long id){
        return vehicleService.getVehiclePrices(id);
    }

    /**
     * Aktualizace vozidla
     * @param vehicle
     * @return
     */
    @RequestMapping(method = RequestMethod.PUT)
    public ResponseEntity<PDBvehicle> editVehicle(@RequestBody PDBvehicle vehicle){
        return vehicleService.updateVehicle(vehicle);
    }

    /**
     * Smazani vozidla dle ID
     * @param id
     * @return
     */
    @RequestMapping(value = "/{id}", method = RequestMethod.DELETE)
    public ResponseEntity<Void> deleteVehicleById(@PathVariable("id") long id){
        return vehicleService.removeVehicle(id);
    }
}
