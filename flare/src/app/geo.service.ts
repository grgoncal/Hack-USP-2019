import { Injectable } from '@angular/core';
import { AngularFirestore } from '@angular/fire/firestore';

import { BehaviorSubject } from 'rxjs/BehaviorSubject'

@Injectable({
  	providedIn: 'root'
})
export class GeoService {

	dbRef: any;
	snData: any;
	geoFire: any;

	hits = new BehaviorSubject([])

  	constructor(private afs: AngularFirestore) {
  		this.dbRef = this.afs.collection('occurrence', ref => ref.where("date", ">=", new Date().getTime() - 10000).orderBy('date', 'desc'));
  		this.snData = this.dbRef.valueChanges();
  		this.snData = this.snData.subscribe(queriedItems => {
        console.log(Object.keys(Object.values(queriedItems)).length)
        if(Object.keys(Object.values(queriedItems)).length > 0){
  			   this.hits.next(Object.values(queriedItems['0']));
        }
			});
  	}
}
