# Stream-o-Vision
#### Your own personal IPTV stations.

## About
The purpose of this project is to provide normal people with their very own streaming IPTV station. By integrating VLC with a scheduler, database connector, and user frendly GUI the project will put the power of any streaming company into your hands. 

## Design philosophy
The intent is to keep a list of issues (bugs and feature requests) that will branch off of a development branch. The dev branch will serve as a platform for testing all commits related to an issue. At certain baselines, the development branch will be merged into the master release branch.

## The challenges of Multicast
The majority of internet connections do not support multicasting beyond your personal router. This project, as is, will only transmit the streams to your own personal network. 

Once the baseline is in Beta, a second project, written in Go-lang (maybe?) will be created that forwards from multicast addresses over unicast to another endpoint, which will fan-out the stream to local multicast addresses. This connector will have the ability to download the channel list and load balance. It will also forward to other requesters of the same channel if the main provider's load is too large. This is to get around the limitations on a standard internet connection.
