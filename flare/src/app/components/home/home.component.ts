import { Component, OnInit } from '@angular/core';
import { GeoService } from '../../geo.service'

import {MatSnackBar} from '@angular/material';

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.css']
})
export class HomeComponent implements OnInit {

  title = 'FLARE';
  lat: number;
  lng: number;
  status = 0;
  statusFlag = true;
  dangerFlag = false;
  sickFlag = false;
  nusp: 0;
  markers = [-23.5511, -46.732];
  markers2 = [-23.550, -46.7331];

  constructor(
    private geo: GeoService,
    private snackbar: MatSnackBar
  ) { }


  ngOnInit() {
    this.getUserLocation();
    this.geo.hits.subscribe(hits => {
        console.log(hits);
        this.status = hits[4];
        this.nusp = hits[3];
        console.log(this.status);
        console.log(this.nusp);
        if(this.status==1){
          this.statusFlag = true;
        }
        else{
          this.statusFlag = false;
        }
        if(this.status==2){
          this.dangerFlag = true;
        }
        else{
          this.dangerFlag = false;
        }
        if(this.status==3){
          this.sickFlag = true;
        }
        else{
          this.sickFlag = false;
        }

        
    });
  }

  private getUserLocation(){
    if(navigator.geolocation){
      navigator.geolocation.getCurrentPosition(position => {
        this.lat = position.coords.latitude;
        this.lng = position.coords.longitude;
      })
    }
  }

}
