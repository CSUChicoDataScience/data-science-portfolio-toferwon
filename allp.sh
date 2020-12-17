#!/bin/bash
for i in {4..1000}
do
   ./p-counter $i www.google.com 0
   ./p-counter $i www.google.com 1
done
for i in {4..1000}
do
   ./p-counter $i www.amazon.com 0
   ./p-counter $i www.amazon.com 1
done
for i in {4..1000}
do
   ./p-counter $i www.csuchico.edu 0
   ./p-counter $i www.csuchico.edu 1
done
for i in {4..1000}
do
   ./p-counter $i www.facebook.com 0
   ./p-counter $i www.facebook.com 1
done
for i in {4..1000}
do
   ./p-counter $i www.twitter.com 0
done
for i in {4..1000}
do
   ./p-counter $i www.instagram.com 0
   ./p-counter $i www.instagram.com 1
done
for i in {4..1000}
do
   ./p-counter $i www.yahoo.com 0
   ./p-counter $i www.yahoo.com 1
done
for i in {4..1000}
do
   ./p-counter $i www.wikipedia.com 0
   ./p-counter $i www.wikipedia.com 1
done
for i in {4..1000}
do
   ./p-counter $i www.tiktok.com 0
   ./p-counter $i www.tiktok.com 1
done
for i in {4..1000}
do
   ./p-counter $i www.youtube.com 0
   ./p-counter $i www.youtube.com 1
done

